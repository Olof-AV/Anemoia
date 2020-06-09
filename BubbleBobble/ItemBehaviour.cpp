#include "pch.h"
#include "ItemBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "Scene.h"

#include "HUDComponent.h"

ItemBehaviour::ItemBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, anemoia::Events attachedEvent)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp }, m_AttachedEvent{ attachedEvent }
{
	m_HasHitFloor = false;
	pRigid->AddIgnoreTag("Player");

	m_Timer = 0.f;
	m_TimerMax = 7.f;
	m_TimerDanger = 5.f;
}

void ItemBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void ItemBehaviour::Update(float elapsedSec)
{
	//If has hit floor, appear clearer + update timers
	if (m_HasHitFloor)
	{
		m_Timer += elapsedSec;
		if (m_Timer > m_TimerMax) //Above timer max -> object should disappear
		{
			GetParent()->GetParentScene()->RemoveChild(GetParent());
		}
		else if (m_Timer > m_TimerDanger) //Above danger time -> object should blink
		{
			m_pTexComp->SetAlpha(255.f * floorf(fmodf(m_Timer * 10.f, 2.f)));
		}
		else //Nothing special
		{
			m_pTexComp->SetAlpha(255.f);
		}
	}
	else //Hasn't hit the ground yet, not tangible
	{
		m_pTexComp->SetAlpha(128.f);
	}
}

void ItemBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void ItemBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	if (pOther->HasTag("Player"))
	{
		Collect(pOther);
	}
	else
	{
		//Has to touch floor before being active
		if (!m_HasHitFloor)
		{
			if (m_pRigid->IsTouchingFloor())
			{
				m_HasHitFloor = true;
				m_pRigid->RemoveIgnoreTag("Player");
			}
		}
	}
}

void ItemBehaviour::Collect(anemoia::GameObject* const pOther)
{
	if (!m_pParent->GetMarkForDelete())
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		pOther->Notify(m_AttachedEvent);
		m_pParent->GetParentScene()->GetObjectWithTag("HUD")->GetComponent<HUDComponent>()->UpdateScores();
	}
}

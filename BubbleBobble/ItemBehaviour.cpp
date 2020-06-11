#include "pch.h"
#include "ItemBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "AnimSpriteComponent.h"
#include "Scene.h"

#include "HUDComponent.h"

ItemBehaviour::ItemBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp, anemoia::Events attachedEvent)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pAnimComp{ pAnimComp }, m_AttachedEvent{ attachedEvent }
{
	m_HasHitFloor = false;
	pRigid->AddIgnoreTag("Player");

	m_Timer = 0.f;
	m_TimerMax = 7.f;
	m_TimerDanger = 5.f;

	m_Collected = false;
	m_CollectedTimer = 0.f;
	m_CollectedTimerMax = 0.125f;
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
			//Visuals are the same, just don't give points to the player
			m_Collected = true;
			m_pAnimComp->SetAnim("Collect");
			m_pRigid->AddIgnoreTag("Player");
		}
		else if (m_Timer > m_TimerDanger) //Above danger time -> object should blink
		{
			m_pAnimComp->SetAlpha(255.f * floorf(fmodf(m_Timer * 10.f, 2.f)));
		}
		else //Nothing special
		{
			m_pAnimComp->SetAlpha(255.f);
		}
	}
	else //Hasn't hit the ground yet, not tangible
	{
		m_pAnimComp->SetAlpha(128.f);
	}

	//If collected, plays anim and disappears
	if (m_Collected)
	{
		m_CollectedTimer += elapsedSec;
		if (m_CollectedTimer > m_CollectedTimerMax)
		{
			m_pParent->GetParentScene()->RemoveChild(m_pParent);
		}
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
	if (!m_Collected)
	{
		m_Collected = true;

		pOther->Notify(m_AttachedEvent);
		m_pParent->GetParentScene()->GetObjectWithTag("HUD")->GetComponent<HUDComponent>()->UpdateScores();

		m_pRigid->AddIgnoreTag("Player");

		m_pAnimComp->SetAnim("Collect");
	}
}

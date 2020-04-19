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
}

void ItemBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);

	//Has to touch floor before being active
	if (!m_HasHitFloor)
	{
		if (m_pRigid->IsTouchingFloor()) { m_HasHitFloor = true; }
		m_pRigid->RemoveIgnoreTag("Player");
	}
}

void ItemBehaviour::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	if (m_HasHitFloor)
	{
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 255.f));
	}
	else
	{
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 128.f));
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

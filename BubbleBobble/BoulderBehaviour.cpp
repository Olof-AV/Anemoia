#include "pch.h"
#include "BoulderBehaviour.h"

#include "TextureComponent.h"
#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"

#include "ZenBehaviour.h"
#include "PlayerBehaviour.h"

#include "AnimSpriteComponent.h"

BoulderBehaviour::BoulderBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp, bool movesLeft)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pAnimComp{ pAnimComp },
	m_MovesLeft{ movesLeft }
{
	//Params
	m_Movement = glm::vec2();
	m_Movement.x = (movesLeft) ? -125.f : 125.f;
	m_pRigid->SetVelocity(m_Movement);

	m_Bursting = false;
	m_BurstTimer = 0.f;
	m_BurstTimerMax = 0.125f;
}

void BoulderBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void BoulderBehaviour::Update(float elapsedSec)
{
	if (m_Bursting)
	{
		m_BurstTimer += elapsedSec;
		if (m_BurstTimer > m_BurstTimerMax)
		{
			m_pParent->GetParentScene()->RemoveChild(m_pParent);
		}
	}
}

void BoulderBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BoulderBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	UNREFERENCED_PARAMETER(pOther);

	if (pOther->HasTag("Player"))
	{
		pOther->GetComponent<PlayerBehaviour>()->Die();
		Burst();
	}
	else
	{
		Burst();
	}
}

void BoulderBehaviour::Burst()
{
	m_Bursting = true;
	m_pRigid->AddIgnoreTag("Player");
	m_pAnimComp->SetAnim("Burst");
	m_pRigid->SetVelocity(glm::vec2{});
}

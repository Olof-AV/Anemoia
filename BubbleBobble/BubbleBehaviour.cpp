#include "pch.h"
#include "BubbleBehaviour.h"

#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "BaseGameScene.h"

#include "ZenBehaviour.h"
#include "MaitaBehaviour.h"

BubbleBehaviour::BubbleBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, bool movesLeft, bool isP1)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid },
	m_MovesLeft{ movesLeft }, m_IsP1{ isP1 }
{
	//Params
	m_Movement = glm::vec2();
	m_Movement.x = (movesLeft) ? -400.f : 400.f;
	m_SlowDownRate = 600.f;
	m_BurstTimer = 0.f;
	m_BurstTimerMax = 7.f;
	m_HorThreshold = 10.f;
	m_FloatRate = -50.f;
	m_UpperLimit = 192.f;
}

void BubbleBehaviour::FixedUpdate(float timeStep)
{
	//Bubble slows down
	if (m_MovesLeft)
	{
		if (m_Movement.x < 0.f)
		{
			m_Movement.x += timeStep * m_SlowDownRate;
		}
		else
		{
			m_Movement.x = 0.f;
		}
	}
	else
	{
		if (m_Movement.x > 0.f)
		{
			m_Movement.x -= timeStep * m_SlowDownRate;
		}
		else
		{
			m_Movement.x = 0.f;
		}
	}
}

void BubbleBehaviour::Update(float elapsedSec)
{
	//If slow enough, will float up
	if (abs(m_Movement).x < m_HorThreshold && GetParent()->GetPosition().y > m_UpperLimit)
	{
		m_Movement.y = m_FloatRate;
	}
	else
	{
		//Prevent bubble from going too far up
		m_Movement.y = 0.f;
	}
	
	//Update
	m_pRigid->SetVelocity(m_Movement);
	
	//Bubble bursts after a given amount of time
	if (m_BurstTimer < m_BurstTimerMax)
	{
		m_BurstTimer += elapsedSec;
	}
	else
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
	}
}

void BubbleBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BubbleBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	UNREFERENCED_PARAMETER(pOther);

	if (pOther->HasTag("Player"))
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
	}
	else if (pOther->HasTag("ZenChan"))
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		pOther->GetComponent<ZenBehaviour>()->GetBubbled(m_IsP1);
	}
	else if (pOther->HasTag("Maita"))
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		pOther->GetComponent<MaitaBehaviour>()->GetBubbled(m_IsP1);
	}
	else
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
	}
}

bool BubbleBehaviour::IsP1() const
{
	return m_IsP1;
}

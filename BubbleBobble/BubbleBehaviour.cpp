#include "pch.h"
#include "BubbleBehaviour.h"

#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "BaseGameScene.h"

#include "ZenBehaviour.h"
#include "MaitaBehaviour.h"

#include "AnimSpriteComponent.h"

BubbleBehaviour::BubbleBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid,
	anemoia::AnimSpriteComponent* const pAnimComp, bool movesLeft, bool isP1)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid },
	m_MovesLeft{ movesLeft }, m_IsP1{ isP1 }, m_pAnimComp(pAnimComp)
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
	m_Bursting = false;

	//Callback to destroy the bubble when finished bursting
	m_pAnimComp->SetBoundFunction([this]() { m_pParent->SetEnabled(false); m_pParent->GetParentScene()->RemoveChild(m_pParent); }, "Poof");
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
	if (abs(m_Movement).x < m_HorThreshold && GetParent()->GetPosition().y > m_UpperLimit && !m_Bursting)
	{
		m_Movement.y = m_FloatRate;
	}
	else
	{
		//Prevent bubble from going too far up
		m_Movement.y = 0.f;
	}
	
	//Update velocity
	m_pRigid->SetVelocity(m_Movement);
	
	//Update burst timer -> switches animations and bursts at the end
	m_BurstTimer += elapsedSec;

	//If bursting, switch anim and ignore all relevant collisions
	if (m_BurstTimer > m_BurstTimerMax)
	{
		Burst();
	}
	else if (m_BurstTimer > m_BurstTimerMax * 0.75f)
	{
		m_pAnimComp->SetAnim("Bubble3");
	}
	else if (m_BurstTimer > m_BurstTimerMax * 0.5f)
	{
		m_pAnimComp->SetAnim("Bubble2");
	}
}

void BubbleBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BubbleBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	if (pOther->HasTag("Player"))
	{
		//Bursts ASAP
		Burst();
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
		//Bursts ASAP
		Burst();
	}
}

bool BubbleBehaviour::IsP1() const
{
	return m_IsP1;
}

void BubbleBehaviour::Burst()
{
	if (!m_Bursting)
	{
		m_pAnimComp->SetAnim("Poof");
		m_pRigid->AddIgnoreTag("Maita");
		m_pRigid->AddIgnoreTag("ZenChan");
		m_pRigid->AddIgnoreTag("Player");
		m_pRigid->AddIgnoreTag("Boulder");
		m_Bursting = true;
		m_Movement = glm::vec2();
	}
}

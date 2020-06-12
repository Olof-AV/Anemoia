#include "pch.h"
#include "BubbleBehaviour.h"

#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "BaseGameScene.h"

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
	
	//Update "state"
	if (!m_Bursting)
	{
		//Update burst timer -> switches animations and bursts at the end
		//If bursting, switch anim and ignore all relevant collisions
		m_BurstTimer += elapsedSec;
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
}

void BubbleBehaviour::LateUpdate(float /*elapsedSec*/)
{

}

void BubbleBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	if (pOther->HasTag("ZenChan") || pOther->HasTag("Maita"))
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		pOther->Notify(((m_IsP1) ? anemoia::Events::BUBBLE_TOUCH_P1 : anemoia::Events::BUBBLE_TOUCH_P2));
	}
	else if(!pOther->HasTag("Player"))
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

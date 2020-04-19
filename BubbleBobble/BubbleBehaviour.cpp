#include "pch.h"
#include "BubbleBehaviour.h"

#include "TextureComponent.h"
#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "BaseGameScene.h"

BubbleBehaviour::BubbleBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool movesLeft)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_MovesLeft{movesLeft}
{
	//Load texs
	const std::string startPath = "Player/";
	m_pTexBubble = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Bubble.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexBubble);

	//Params
	m_Movement = glm::vec2();
	m_Movement.x = (movesLeft) ? -200.f : 200.f;
	m_SlowDownRate = 100.f;
	m_BurstTimer = 0.f;
	m_BurstTimerMax = 7.f;
	m_HorThreshold = 10.f;
}

void BubbleBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void BubbleBehaviour::Update(float elapsedSec)
{
	//Bubble slows down
	if (m_MovesLeft)
	{
		if (m_Movement.x < 0.f)
		{
			m_Movement.x += elapsedSec * m_SlowDownRate;
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
			m_Movement.x -= elapsedSec * m_SlowDownRate;
		}
		else
		{
			m_Movement.x = 0.f;
		}
	}


	//If slow enough, will float up
	if (abs(m_Movement).x < m_HorThreshold && GetParent()->GetPosition().y > 192.f)
	{
		m_Movement.y = -50.f;
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
		m_pParent->GetParentScene()->RemoveChild(pOther);
		static_cast<BaseGameScene*>(m_pParent->GetParentScene())->NotifyEnemyDeath(pOther);
	}
	else
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
	}
}

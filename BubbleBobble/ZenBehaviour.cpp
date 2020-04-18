#include "pch.h"
#include "ZenBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"

#include "Scene.h"
#include "ResourceManager.h"

#include "PlayerBehaviour.h"

ZenBehaviour::ZenBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_IsDead{ false }
{
	m_InputDir = glm::vec2{};

	//Load texs
	const std::string startPath = "Enemies/ZenChan/";
	m_pTexIdle = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Run.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexIdle);

	//Params
	m_MovSpeed = 200.f;
	m_JumpForce = 600.f;

	//Player
	m_pPlayer = pParent->GetParentScene()->GetObjectWithTag("Player");
}

void ZenBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void ZenBehaviour::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	//Test stuff
	const glm::vec3 playerPos = m_pPlayer->GetPosition();
	const glm::vec3 myPos = m_pParent->GetPosition();
	glm::vec2 vel = m_pRigid->GetVelocity();

	if (playerPos.x < myPos.x)
	{
		vel.x = -m_MovSpeed;
		m_pRigid->SetVelocity(vel);
	}
	else
	{
		vel.x = m_MovSpeed;
		m_pRigid->SetVelocity(vel);
	}
}

void ZenBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void ZenBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	if (pOther == m_pPlayer)
	{
		m_pPlayer->GetComponent<PlayerBehaviour>()->Die();
	}
}

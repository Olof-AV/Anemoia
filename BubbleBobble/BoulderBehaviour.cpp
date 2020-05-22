#include "pch.h"
#include "BoulderBehaviour.h"

#include "TextureComponent.h"
#include "RigidBodyComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"

#include "ZenBehaviour.h"
#include "PlayerBehaviour.h"

BoulderBehaviour::BoulderBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool movesLeft)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_MovesLeft{ movesLeft }
{
	//Load texs
	const std::string startPath = "Enemies/Maita/";
	m_pTexBoulder = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Boulder.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexBoulder);

	//Params
	m_Movement = glm::vec2();
	m_Movement.x = (movesLeft) ? -100.f : 100.f;
	m_pRigid->SetVelocity(m_Movement);
}

void BoulderBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void BoulderBehaviour::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
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
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		pOther->GetComponent<PlayerBehaviour>()->Die();

	}
	else
	{
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
	}
}

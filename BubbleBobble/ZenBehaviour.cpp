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
	m_MovSpeed = 100.f;
	m_JumpForce = 600.f;
	m_HorThreshold = 1.f;
	m_VerThreshold = 30.f;

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

	//To compare positions
	const glm::vec3 playerPos = m_pPlayer->GetPosition();
	const glm::vec3 myPos = m_pParent->GetPosition();

	//Current parameters
	const float horDistance = myPos.x - playerPos.x;
	const float verDistance = myPos.y - playerPos.y;
	glm::vec2 vel = m_pRigid->GetVelocity();

	//AI doesn't do anything if not touching floor
	if (m_pRigid->IsTouchingFloor())
	{
		//If vertical distance needs to be examined
		if (abs(verDistance) > m_VerThreshold)
		{
			//If player is above
			if (verDistance > 0.f)
			{
				//Jump
				if (abs(horDistance) < m_HorThreshold)
				{
					m_InputDir.y = -1.f;
					m_InputDir.x = 0.f;
				}
				//If not moving, start moving towards player
				else
				{
					m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
				}

			}
			//Player is below
			else
			{
				//Don't jump
				m_InputDir.y = 0.f;

				//If not moving, start moving towards player
				if (m_InputDir.x == 0.f || abs(vel.x) < 1.f)
				{
					m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
				}
			}
		}
		//Same level
		else
		{
			//Just move towards player
			m_InputDir.y = 0.f;
			m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
		}
	}

	//Inputs are used on rigidbody
	if (m_pRigid->IsTouchingFloor())
	{
		m_pRigid->AddVelocity(glm::vec2(0.f, m_InputDir.y * m_JumpForce));
	}
	vel = m_pRigid->GetVelocity();
	m_pRigid->SetVelocity(glm::vec2(m_InputDir.x * m_MovSpeed, vel.y));

	//Flip sprite based on direction
	anemoia::Transform transform = m_pTexComp->GetTransform();
	if (m_InputDir.x > 0.1f)
	{
		transform.SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
		m_pTexComp->SetTransform(transform);
	}
	else if (m_InputDir.x < -0.1f)
	{
		transform.SetFlip(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
		m_pTexComp->SetTransform(transform);
	}
}

void ZenBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void ZenBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	//Extra safety check to ensure player actually dies
	if (pOther == m_pPlayer)
	{
		m_pPlayer->GetComponent<PlayerBehaviour>()->Die();
	}
}

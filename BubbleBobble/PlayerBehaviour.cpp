#include "pch.h"
#include "PlayerBehaviour.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "ColliderComponent.h"
#include "BubbleBehaviour.h"

#include "ResourceManager.h"

#include "Scene.h"

#include "ZenBehaviour.h"
#include "MaitaBehaviour.h"
#include "ItemBehaviour.h"

#include "Locator.h"
#include "BubbleBobbleGame.h"

PlayerBehaviour::PlayerBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool isP2)
	: BaseComponent(pParent, anemoia::Transform()),
	m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_IsDead{ false }, m_CurrentState{ PlayerState::idle },
	m_IsP1{!isP2}
{
	//Controller ID depending on P1/P2
	const int controllerId = (isP2) ? 1 : 0;

	//Setup input
	anemoia::InputManager* const pInput = anemoia::InputManager::GetInstance();
	pInput->RegisterCommand(new anemoia::Command("MoveLeft", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_DPAD_LEFT, ((isP2) ? VK_NUMPAD4 : VK_LEFT), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x += -1.f;
	}));
	pInput->RegisterCommand(new anemoia::Command("MoveRight", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_DPAD_RIGHT, ((isP2) ? VK_NUMPAD6 : VK_RIGHT), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x += 1.f;
	}));
	pInput->RegisterCommand(new anemoia::Command("Jump", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_A, ((isP2) ? VK_NUMPAD8 : 'Z'), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.y += -1.f;
	}));
	pInput->RegisterCommand(new anemoia::Command("Shoot", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_X, ((isP2) ? VK_NUMPAD5 : 'X'), anemoia::ButtonState::Down, std::bind(&PlayerBehaviour::ShootBubble, this) ));

	//Load textures
	const std::string startPath = ((isP2) ? "Player/Bobby/" : "Player/Bubby/");
	m_pTexIdle = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Idle.png");
	m_pTexShoot = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Shoot.png");

	//Set base state
	SetState(PlayerState::idle);

	//Params
	m_InitialPos = pParent->GetPosition();
	m_InputDir = glm::vec2{};
	m_MovSpeed = 200.f;
	m_JumpForce = 600.f;
	m_IsInvincible = false;

	m_InvincibilityTimer = 0.f;
	m_InvincibilityTimerMax = 3.f;

	m_ShootCoolTimer = 0.f;
	m_ShootCoolTimerMax = 0.75f;
}

void PlayerBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void PlayerBehaviour::Update(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case PlayerState::idle:
		HandleMovement();
		HandleInvincibilityTimer(elapsedSec);

		break;

	case PlayerState::shoot:
		HandleMovement();
		HandleInvincibilityTimer(elapsedSec);
		
		//Cooldown eventually dissipates
		m_ShootCoolTimer += elapsedSec;
		if (m_ShootCoolTimer > m_ShootCoolTimerMax) { SetState(PlayerState::idle); }

		break;
	}
}

void PlayerBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void PlayerBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	BaseComponent::OnCollide(pOther);

	if (pOther->HasTag("ZenChan"))
	{
		pOther->GetComponent<ZenBehaviour>()->PlayerTouch(m_pParent);
	}
	if (pOther->HasTag("Maita"))
	{
		pOther->GetComponent<MaitaBehaviour>()->PlayerTouch(m_pParent);
	}
	else if (pOther->HasTag("Bubble"))
	{
		if (m_InputDir.y < 0.f)
		{
			//Bounces on top of bubble
		}
		else
		{
			m_pParent->GetParentScene()->RemoveChild(pOther);
		}
	}
	else if (pOther->HasTag("Boulder"))
	{
		m_pParent->GetParentScene()->RemoveChild(pOther);
		Die();
	}
	else if (pOther->HasTag("Treasure"))
	{
		pOther->GetComponent<ItemBehaviour>()->Collect(m_pParent);
	}
}

void PlayerBehaviour::HandleMovement()
{
	//Teleport on death
	if (m_IsDead)
	{
		m_IsInvincible = true;
		m_IsDead = false;
		m_pRigid->Move(m_InitialPos);
		m_pRigid->SetVelocity(glm::vec2(0.f, 0.f));
	}
	//Otherwise handle mov related transform stuff + movement
	else
	{
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

		//Move, will need to be remade because this is just testing stuff
		const glm::vec2 velocity = m_pRigid->GetVelocity();
		m_pRigid->SetVelocity(glm::vec2(m_InputDir.x * m_MovSpeed, velocity.y));

		//Jump if touching floor
		if (m_pRigid->IsTouchingFloor())
		{
			m_pRigid->AddVelocity(glm::vec2(0.f, m_InputDir.y * m_JumpForce));
		}

		//Reset
		m_InputDir = glm::vec2{};
	}
}

void PlayerBehaviour::HandleInvincibilityTimer(float elapsedSec)
{
	//Invincibility timer
	if (m_IsInvincible)
	{
		if (m_InvincibilityTimer < m_InvincibilityTimerMax)
		{
			m_InvincibilityTimer += elapsedSec;
		}
		else
		{
			m_InvincibilityTimer = 0.f;
			m_IsInvincible = false;

			m_pRigid->RemoveIgnoreTag("ZenChan");
			m_pRigid->RemoveIgnoreTag("Maita");
		}
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 128.f));
	}
	else
	{
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 255.f));
	}
}

void PlayerBehaviour::Die()
{
	if (!m_IsDead && !m_IsInvincible)
	{
		const int lives = static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->GetLives(m_IsP1);

		if (lives == 0)
		{
			std::cout << "100% dead\n";
			m_pParent->GetParentScene()->RemoveChild(m_pParent);
		}
		else
		{
			std::cout << "Die here\n";

			m_pRigid->AddIgnoreTag("ZenChan");
			m_pRigid->AddIgnoreTag("Maita");
		}

		m_IsDead = true;
		m_pParent->Notify(anemoia::Events::PLAYER_DEATH);
	}
}

void PlayerBehaviour::ShootBubble()
{
	//Can't shoot if cooldown is in effect, or if invincibility is in effect
	if (m_CurrentState != PlayerState::shoot && !m_IsInvincible)
	{
		//State
		SetState(PlayerState::shoot);

		//Spawn bubble
		{
			//Params
			const bool isLookingLeft = (m_pTexComp->GetTransform().GetFlip() == SDL_FLIP_HORIZONTAL) ? true : false;
			anemoia::Scene* pScene = GetParent()->GetParentScene();

			//Obj
			anemoia::GameObject* const pObj = new anemoia::GameObject(pScene);
			glm::vec3 finalPos = GetParent()->GetPosition();
			finalPos += ((isLookingLeft) ? glm::vec3(-60.f, -28.f, 0.f) : glm::vec3(60.f, -28.f, 0.f));
			pObj->SetPosition(finalPos);

			//Transform
			anemoia::Transform transform = anemoia::Transform(glm::vec3(), glm::vec2(0.5f, 0.5f));

			//Collider 
			anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pObj, transform, glm::vec2(48.f, 48.f), true);
			pObj->AddComponent(pColl);

			//Texture
			anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pObj, transform, nullptr);
			pObj->AddComponent(pTexComp);

			//Rigid
			anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl, 0.f);
			pObj->AddComponent(pRigid);
			pRigid->AddIgnoreTag("Treasure");
			pRigid->AddIgnoreTag("Bubble");
			pRigid->AddIgnoreTag("Boulder");

			//Bubble behaviour
			BubbleBehaviour* const pBehaviour = new BubbleBehaviour(pObj, pRigid, pTexComp, isLookingLeft);
			pObj->AddComponent(pBehaviour);

			//Tag
			pObj->AddTag("Bubble");

			//Add to scene
			pScene->AddChild(pObj);
		}
	}
}

PlayerState PlayerBehaviour::GetState() const
{
	return m_CurrentState;
}

void PlayerBehaviour::SetState(PlayerState newState)
{
	//Machine according to new state
	switch (newState)
	{
	case PlayerState::idle:
		m_pTexComp->SetTexture(m_pTexIdle);

		break;

	case PlayerState::shoot:
		m_pTexComp->SetTexture(m_pTexShoot);
		m_ShootCoolTimer = 0.f;

		break;
	}

	//Don't forget to change state
	m_CurrentState = newState;
}

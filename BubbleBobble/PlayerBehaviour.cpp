#include "pch.h"
#include "PlayerBehaviour.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

#include "RigidBodyComponent.h"
#include "AnimSpriteComponent.h"
#include "ColliderComponent.h"
#include "BubbleBehaviour.h"
#include "BoulderBehaviour.h"

#include "ResourceManager.h"

#include "Scene.h"

#include "ZenBehaviour.h"
#include "MaitaBehaviour.h"
#include "ItemBehaviour.h"

#include "Locator.h"
#include "BubbleBobbleGame.h"

#include "Sound.h"

PlayerBehaviour::PlayerBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp, bool isP2)
	: BaseComponent(pParent, anemoia::Transform()),
	m_pRigid{ pRigid }, m_pAnimComp{ pAnimComp },
	m_IsDead{ false }, m_CurrentState{ PlayerState::idle },
	m_IsP1{!isP2}
{
	//Controller ID depending on P1/P2
	const int controllerId = (isP2) ? 1 : 0;

	//Setup input
	anemoia::InputManager* const pInput = anemoia::InputManager::GetInstance();
	pInput->RegisterCommand(m_pCommand_Left = new anemoia::Command("MoveLeft", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_DPAD_LEFT, ((isP2) ? VK_NUMPAD4 : VK_LEFT), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x += -1.f;
	}));
	pInput->RegisterCommand(m_pCommand_Right = new anemoia::Command("MoveRight", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_DPAD_RIGHT, ((isP2) ? VK_NUMPAD6 : VK_RIGHT), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x += 1.f;
	}));
	pInput->RegisterCommand(m_pCommand_Jump = new anemoia::Command("Jump", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_A, ((isP2) ? VK_NUMPAD8 : 'Z'), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.y += -1.f;
	}));
	pInput->RegisterCommand(m_pCommand_Shoot = new anemoia::Command("Shoot", pParent->GetParentScene(), controllerId,
		XINPUT_GAMEPAD_X, ((isP2) ? VK_NUMPAD5 : 'X'), anemoia::ButtonState::Down, std::bind(&PlayerBehaviour::ShootBubble, this) ));

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
	m_ShootCoolTimerMax = 0.55f;

	//Bind death to end of death anim
	m_pAnimComp->SetBoundFunction([this]() { Die(); }, "Death");

	//Load sounds
	{
		m_pSound_Jump = anemoia::ResourceManager::GetInstance()->LoadSound("Player/Jump.wav");
		m_pSound_JumpBubble = anemoia::ResourceManager::GetInstance()->LoadSound("Player/Jump_Bubble.wav");
		m_pSound_Shoot = anemoia::ResourceManager::GetInstance()->LoadSound("Player/Shoot.wav");
	}
}

PlayerBehaviour::~PlayerBehaviour()
{
	delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Left);
	delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Right);
	delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Jump);
	delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Shoot);
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
		if (m_InputDir.x > 0.1f || m_InputDir.x < -0.1f) //Switch to run anim
		{
			SetState(PlayerState::run);
		}

		break;

	case PlayerState::run:
		if (m_InputDir.x < 0.1f && m_InputDir.x > -0.1f) //Switch to idle anim
		{
			SetState(PlayerState::idle);
		}

		break;

	case PlayerState::shoot:
		//Cooldown eventually dissipates
		m_ShootCoolTimer += elapsedSec;
		if (m_ShootCoolTimer > m_ShootCoolTimerMax) { SetState(PlayerState::idle); }

		break;

	case PlayerState::death:

		break;
	}

	//Common to all states
	HandleMovement();
	HandleInvincibilityTimer(elapsedSec);
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
		ZenBehaviour* const pBehaviour = pOther->GetComponent<ZenBehaviour>();
		if (pBehaviour->GetState() == ZenState::bubble && m_InputDir.y < 0.f && m_pRigid->IsTouchingFloor())
		{
			//Bounces on top of bubble
			m_pSound_JumpBubble->Play(0);
		}
		else
		{
			pBehaviour->PlayerTouch(m_pParent);
		}
	}
	if (pOther->HasTag("Maita"))
	{
		MaitaBehaviour* const pBehaviour = pOther->GetComponent<MaitaBehaviour>();
		if (pBehaviour->GetState() == MaitaState::bubble && m_InputDir.y < 0.f && m_pRigid->IsTouchingFloor())
		{
			//Bounces on top of bubble
			m_pSound_JumpBubble->Play(0);
		}
		else
		{
			pBehaviour->PlayerTouch(m_pParent);
		}
	}
	else if (pOther->HasTag("Bubble"))
	{
		if (m_InputDir.y < 0.f)
		{
			//Bounces on top of bubble
			m_pSound_JumpBubble->Play(0);
		}
		else
		{
			pOther->GetComponent<BubbleBehaviour>()->Burst();
		}
	}
	/*else if (pOther->HasTag("Boulder"))
	{
		pOther->GetComponent<BoulderBehaviour>()->Burst();
		m_pParent->Notify(anemoia::Events::DAMAGE_TAKEN);
	}*/
	else if (pOther->HasTag("Treasure"))
	{
		m_pRigid->SetTouchingFloor(false); //This prevents players from jumping on top of collectibles
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
	else if(m_CurrentState != PlayerState::death)
	{
		anemoia::Transform transform = m_pAnimComp->GetTransform();
		if (m_InputDir.x > 0.1f)
		{
			transform.SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
			m_pAnimComp->SetTransform(transform);
		}
		else if (m_InputDir.x < -0.1f)
		{
			transform.SetFlip(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			m_pAnimComp->SetTransform(transform);
		}

		//Set lateral mov speed
		m_pRigid->SetVelocity(glm::vec2(m_InputDir.x * m_MovSpeed, m_pRigid->GetVelocity().y));

		//Jump if touching floor
		if (m_pRigid->IsTouchingFloor() && m_InputDir.y != 0.f)
		{
			m_pSound_Jump->Play(0);
			m_pRigid->SetVelocity(glm::vec2(m_pRigid->GetVelocity().x, m_InputDir.y * m_JumpForce));
		}

		//Reset
		m_InputDir = glm::vec2{};
	}
	else
	{
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

			m_pRigid->RemoveIgnoreTag("Boulder");
			m_pRigid->RemoveIgnoreTag("ZenChan");
			m_pRigid->RemoveIgnoreTag("Maita");
		}
		m_pAnimComp->SetAlpha(128.f);
	}
	else
	{
		m_pAnimComp->SetAlpha(255.f);
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
			m_pParent->SetEnabled(false);
		}
		else
		{
			std::cout << "Die here\n";

			m_pRigid->AddIgnoreTag("Boulder");
			m_pRigid->AddIgnoreTag("ZenChan");
			m_pRigid->AddIgnoreTag("Maita");
		}

		m_IsDead = true;
		m_pParent->Notify(anemoia::Events::PLAYER_DEATH);

		SetState(PlayerState::idle);
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
			const bool isLookingLeft = (m_pAnimComp->GetTransform().GetFlip() == SDL_FLIP_HORIZONTAL) ? true : false;
			anemoia::Scene* pScene = GetParent()->GetParentScene();

			//Obj
			anemoia::GameObject* const pObj = new anemoia::GameObject(pScene);
			glm::vec3 finalPos = GetParent()->GetPosition();
			finalPos += ((isLookingLeft) ? glm::vec3(-60.f, -28.f, 0.f) : glm::vec3(60.f, -28.f, 0.f));
			pObj->SetPosition(finalPos);

			//Transform
			anemoia::Transform transform = anemoia::Transform(glm::vec3(), glm::vec2(0.5f, 0.5f));

			//Collider 
			anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pObj, transform, glm::vec2(48.f, 48.f), true, false);
			pObj->AddComponent(pColl);

			//Animation
			anemoia::AnimSpriteComponent* const pAnimSprite =
				new anemoia::AnimSpriteComponent(pObj, transform, "Anims/Bubble.txt", ((m_IsP1) ? "Bubble" : "Bubble_Alt"));
			pObj->AddComponent(pAnimSprite);

			//Rigid
			anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl, 0.f);
			pObj->AddComponent(pRigid);
			pRigid->AddIgnoreTag("Treasure");
			pRigid->AddIgnoreTag("Bubble");
			pRigid->AddIgnoreTag("Boulder");

			//Bubble behaviour
			BubbleBehaviour* const pBehaviour = new BubbleBehaviour(pObj, pRigid, pAnimSprite, isLookingLeft, m_IsP1);
			pObj->AddComponent(pBehaviour);

			//Tag
			pObj->AddTag("Bubble");

			//Add to scene
			pScene->AddChild(pObj);

			//Play sound
			m_pSound_Shoot->Play(0);
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
		m_pAnimComp->SetAnim("Idle");

		break;

	case PlayerState::run:
		m_pAnimComp->SetAnim("Run");

		break;

	case PlayerState::shoot:
		m_pAnimComp->SetAnim("Shoot");
		m_pAnimComp->ResetCurrentAnim(); //Looks much smoother this way
		m_ShootCoolTimer = 0.f;

		break;

	case PlayerState::death:
		m_pAnimComp->SetAnim("Death");
		m_pAnimComp->ResetCurrentAnim();

		const glm::vec2 &vel = m_pRigid->GetVelocity();
		m_pRigid->SetVelocity(glm::vec2{0.f, vel.y});

		break;
	}

	//Don't forget to change state
	m_CurrentState = newState;
}

bool PlayerBehaviour::IsHoldingJump() const
{
	return m_InputDir.y > 0.f;
}

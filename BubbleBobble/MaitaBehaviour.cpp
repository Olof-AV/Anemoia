#include "pch.h"
#include "MaitaBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "ColliderComponent.h"
#include "ItemBehaviour.h"

#include "Scene.h"
#include "BaseGameScene.h"
#include "ResourceManager.h"

#include "PlayerBehaviour.h"
#include "BoulderBehaviour.h"

#include "InputManager.h"
#include "Command.h"

#include "AIManager.h"

MaitaBehaviour::MaitaBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool isPlayer)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_IsDead{ false }, m_CurrentState{ MaitaState::run }, m_IsPlayer(isPlayer)
{
	m_InputDir = glm::vec2{};

	//Load texs
	const std::string startPath = "Enemies/Maita/";
	m_pTexRun = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Run.png");
	m_pTexBubble = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Bubble.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexRun);

	//Params
	m_MovSpeed = 100.f;
	m_JumpForce = 600.f;
	m_HorThreshold = 50.f;
	m_VerThreshold = 30.f;
	m_FloatRate = -75.f;

	m_BubbleBurstTimer = 0.f;
	m_BubbleBurstTimerMax = 7.f;
	
	m_BoulderCooldown = false;
	m_BoulderTimer = 0.f;
	m_BoulderTimerMax = 5.f;

	//Player
	m_pPlayer = pParent->GetParentScene()->GetObjectWithTag("Player");

	//Add controls?
	if (isPlayer)
	{
		anemoia::InputManager* const pInput = anemoia::InputManager::GetInstance();
		pInput->RegisterCommand(m_pCommand_Left = new anemoia::Command("MoveLeft", pParent->GetParentScene(), 1,
			XINPUT_GAMEPAD_DPAD_LEFT, VK_NUMPAD4, anemoia::ButtonState::Hold, [this]()
			{
				m_InputDir.x += -1.f;
			}));
		pInput->RegisterCommand(m_pCommand_Right = new anemoia::Command("MoveRight", pParent->GetParentScene(), 1,
			XINPUT_GAMEPAD_DPAD_RIGHT, VK_NUMPAD6, anemoia::ButtonState::Hold, [this]()
			{
				m_InputDir.x += 1.f;
			}));
		pInput->RegisterCommand(m_pCommand_Jump = new anemoia::Command("Jump", pParent->GetParentScene(), 1,
			XINPUT_GAMEPAD_A, VK_NUMPAD8, anemoia::ButtonState::Hold, [this]()
			{
				m_InputDir.y += -1.f;
			}));
		pInput->RegisterCommand(m_pCommand_Shoot = new anemoia::Command("Shoot", pParent->GetParentScene(), 1,
			XINPUT_GAMEPAD_X, VK_NUMPAD5, anemoia::ButtonState::Down, std::bind(&MaitaBehaviour::ShootBoulder, this)));
	}
	else
	{
		//AI
		anemoia::BoundFunc func;
		func.func = std::bind(&MaitaBehaviour::RunAI, this);
		func.pParent = pParent;
		func.pScene = pParent->GetParentScene();
		anemoia::AIManager::GetInstance()->RegisterFunction(func);
	}
}

MaitaBehaviour::~MaitaBehaviour()
{
	//AI cleanup
	if (!m_IsPlayer)
	{
		anemoia::BoundFunc func;
		func.func = std::bind(&MaitaBehaviour::RunAI, this);
		func.pParent = GetParent();
		func.pScene = func.pParent->GetParentScene();
		anemoia::AIManager::GetInstance()->UnRegisterFunction(func);
	}
	else
	{
		delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Left);
		delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Right);
		delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Jump);
		delete anemoia::InputManager::GetInstance()->UnregisterCommand(m_pCommand_Shoot);
	}
}

void MaitaBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void MaitaBehaviour::Update(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case MaitaState::run:
		HandleMovement();

		if (m_BoulderCooldown)
		{
			m_BoulderTimer += elapsedSec;
			if (m_BoulderTimer > m_BoulderTimerMax) { m_BoulderCooldown = false; m_BoulderTimer = 0.f; }
		}
		else if (m_ShootRequested)
		{
			m_ShootRequested = false;
			ShootBoulder();
		}

		break;

	case MaitaState::bubble:
		HandleBubbleMov();

		m_BubbleBurstTimer += elapsedSec;
		if (m_BubbleBurstTimer > m_BubbleBurstTimerMax) { SetState(MaitaState::run); }

		break;
	}
}

void MaitaBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void MaitaBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	//Extra safety check to ensure player actually dies
	if (pOther->HasTag("Player"))
	{
		PlayerTouch(pOther);
	}
	//Get bubbled
	else if (pOther->HasTag("Bubble"))
	{
		m_pParent->GetParentScene()->RemoveChild(pOther);
		SetState(MaitaState::bubble);
	}
}

MaitaState MaitaBehaviour::GetState() const
{
	return m_CurrentState;
}

void MaitaBehaviour::SetState(MaitaState newState)
{
	//Machine on new state
	switch (newState)
	{
	case MaitaState::run:
		m_pTexComp->SetTexture(m_pTexRun);

		break;

	case MaitaState::bubble:
		m_pTexComp->SetTexture(m_pTexBubble);
		m_BubbleBurstTimer = 0.f;

		break;
	}

	//Don't forget to change
	m_CurrentState = newState;
}

void MaitaBehaviour::HandleMovement()
{
	glm::vec2 vel = m_pRigid->GetVelocity();
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

	//Reset
	if (m_IsPlayer) { m_InputDir = glm::vec2(); }
}

void MaitaBehaviour::HandleBubbleMov()
{
	//Just floats up a little + reset input
	m_pRigid->SetVelocity(glm::vec2(0.f, m_FloatRate));
	m_InputDir = glm::vec2();
}

void MaitaBehaviour::PlayerTouch(anemoia::GameObject* const pOther)
{
	//If touched while bubbled, this ZenChan dies
	if (m_CurrentState == MaitaState::bubble)
	{
		if (!m_pParent->GetMarkForDelete())
		{
			//Spawn watermelon
			{
				//Obj
				anemoia::GameObject* const pObj = new anemoia::GameObject(m_pParent->GetParentScene());
				pObj->SetPosition(m_pParent->GetPosition());

				//Texture
				anemoia::Texture2D* const pTex = anemoia::ResourceManager::GetInstance()->LoadTexture("Items/Fries.png");
				anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pObj, m_pTexComp->GetTransform(), pTex);
				pObj->AddComponent(pTexComp);

				//Collider
				anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pObj, m_pTexComp->GetTransform(), glm::vec2(48.f, 48.f));
				pObj->AddComponent(pColl);

				//Rigid body
				anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl);
				pObj->AddComponent(pRigid);
				pRigid->AddIgnoreTag("ZenChan");
				pRigid->AddIgnoreTag("Bubble");
				pRigid->AddIgnoreTag("Treasure");

				//Behaviour
				ItemBehaviour* const pBehaviour = new ItemBehaviour(pObj, pRigid, pTexComp, anemoia::Events::PLAYER_OBTAIN_FRIES);
				pObj->AddComponent(pBehaviour);

				//Tag
				pObj->AddTag("Treasure");

				//Add to scene
				m_pParent->GetParentScene()->AddChild(pObj);
			}
		}

		//Remove enemy
		m_pParent->GetParentScene()->RemoveChild(m_pParent);
		static_cast<BaseGameScene*>(m_pParent->GetParentScene())->NotifyEnemyDeath(m_pParent);
	}
	//Otherwise, the player dies
	else
	{
		pOther->GetComponent<PlayerBehaviour>()->Die();
	}
}

void MaitaBehaviour::ShootBoulder()
{
	//Can't shoot if cooldown is in effect, or if bubbled up
	if (m_CurrentState != MaitaState::bubble && !m_BoulderCooldown)
	{
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
			pRigid->AddIgnoreTag("Maita");
			pRigid->AddIgnoreTag("ZenChan");

			//Bubble behaviour
			BoulderBehaviour* const pBehaviour = new BoulderBehaviour(pObj, pRigid, pTexComp, isLookingLeft);
			pObj->AddComponent(pBehaviour);

			//Tag
			pObj->AddTag("Boulder");

			//Add to scene
			pScene->AddChild(pObj);
		}

		m_BoulderCooldown = true;
	}
}

void MaitaBehaviour::RunAI()
{
	//AI
	if (m_CurrentState != MaitaState::bubble)
	{
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
				m_ShootRequested = false;

				//If player is above
				if (verDistance > 0.f)
				{
					//Jump
					if (abs(horDistance) < m_HorThreshold)
					{
						m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
						m_InputDir.y = -1.f;
					}
					//If not moving, start moving towards player
					else
					{
						m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
						m_InputDir.y = 0.f;
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
						//m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
						m_InputDir.x = (rand() % 2) ? -1.f : 1.f;
					}
				}
			}
			//Same level
			else
			{
				//Just move towards player
				m_InputDir.y = 0.f;
				m_InputDir.x = (playerPos.x < myPos.x) ? -1.f : 1.f;
				m_ShootRequested = true;
			}
		}
	}
	else
	{
		m_InputDir = glm::vec2();
	}
}

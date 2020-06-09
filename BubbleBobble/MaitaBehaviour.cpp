#include "pch.h"
#include "MaitaBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "AnimSpriteComponent.h"
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
#include <algorithm>

MaitaBehaviour::MaitaBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp, bool isPlayer)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pAnimComp{ pAnimComp },
	m_IsDead{ false }, m_CurrentState{ MaitaState::run }, m_IsPlayer(isPlayer)
{
	m_InputDir = glm::vec2{};

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

	m_AttackCharge = 0.f;
	m_AttackChargeMax = 0.4f;

	//Players
	m_Targets = pParent->GetParentScene()->GetObjectsWithTag("Player");

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
			XINPUT_GAMEPAD_X, VK_NUMPAD5, anemoia::ButtonState::Down, std::bind(&MaitaBehaviour::PrepareBoulder, this)));
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

		//Can't shoot with cooldown
		if (m_BoulderCooldown)
		{
			m_BoulderTimer += elapsedSec;
			if (m_BoulderTimer > m_BoulderTimerMax) { m_BoulderCooldown = false; m_BoulderTimer = 0.f; }
		}
		//If no cooldown, shoot is allowed
		else if (m_ShootRequested)
		{
			m_ShootRequested = false;
			PrepareBoulder();
		}

		break;

	case MaitaState::attack:
		HandleMovement();

		m_AttackCharge += elapsedSec; //Attack takes a little while to charge, before it shoots
		if (m_AttackCharge > m_AttackChargeMax)
		{
			m_AttackCharge = 0.f;
			SetState(MaitaState::run);
			ShootBoulder();
		}

		break;

	case MaitaState::bubble:
		HandleBubbleMov();

		m_BubbleBurstTimer += elapsedSec; //After a while, Maita bursts out of bubble
		if (m_BubbleBurstTimer > m_BubbleBurstTimerMax) { SetState(MaitaState::run); }
		else if (m_BubbleBurstTimer > m_BubbleBurstTimerMax * 0.75f) { m_pAnimComp->SetAnim("Bubble3"); }
		else if (m_BubbleBurstTimer > m_BubbleBurstTimerMax * 0.5f) { m_pAnimComp->SetAnim("Bubble2"); }

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
		m_pAnimComp->SetAnim("Run");
		m_AttackCharge = 0.f;

		break;

	case MaitaState::attack:
		m_pAnimComp->SetAnim("Attack");
		m_pAnimComp->ResetCurrentAnim();
		m_AttackCharge = 0.f;

		break;

	case MaitaState::bubble:
		m_pAnimComp->SetAnim("Bubble");
		m_BubbleBurstTimer = 0.f;
		m_AttackCharge = 0.f;

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
				anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pObj, m_pAnimComp->GetTransform(), pTex);
				pObj->AddComponent(pTexComp);

				//Collider
				anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pObj, m_pAnimComp->GetTransform(), glm::vec2(48.f, 48.f), true, false);
				pObj->AddComponent(pColl);

				//Rigid body
				anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl);
				pObj->AddComponent(pRigid);
				pRigid->AddIgnoreTag("ZenChan");
				pRigid->AddIgnoreTag("Maita");
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
		m_pParent->SetEnabled(false);
		static_cast<BaseGameScene*>(m_pParent->GetParentScene())->NotifyEnemyDeath(m_pParent);
	}
	//Otherwise, the player dies
	else
	{
		pOther->GetComponent<PlayerBehaviour>()->Die();
	}
}

void MaitaBehaviour::PrepareBoulder()
{
	//Only attack if allowed, also can't prepare again while preparing
	if (m_CurrentState != MaitaState::bubble && m_CurrentState != MaitaState::attack && !m_BoulderCooldown)
	{
		SetState(MaitaState::attack);
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
			const bool isLookingLeft = (m_pAnimComp->GetTransform().GetFlip() == SDL_FLIP_HORIZONTAL) ? true : false;
			anemoia::Scene* pScene = GetParent()->GetParentScene();

			//Obj
			anemoia::GameObject* const pObj = new anemoia::GameObject(pScene);
			glm::vec3 finalPos = GetParent()->GetPosition();
			finalPos += ((isLookingLeft) ? glm::vec3(-30.f, -28.f, 0.f) : glm::vec3(30.f, -28.f, 0.f));
			pObj->SetPosition(finalPos);

			//Transform
			anemoia::Transform transform = anemoia::Transform(glm::vec3(), glm::vec2(0.5f, 0.5f));

			//Collider 
			anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pObj, transform, glm::vec2(48.f, 48.f), true, false);
			pObj->AddComponent(pColl);

			//Texture
			anemoia::AnimSpriteComponent* const pAnimComp = new anemoia::AnimSpriteComponent(pObj, transform, "Anims/Boulder.txt", "Boulder");
			pObj->AddComponent(pAnimComp);

			//Rigid
			anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl, 0.f);
			pObj->AddComponent(pRigid);
			pRigid->AddIgnoreTag("Treasure");
			pRigid->AddIgnoreTag("Bubble");
			pRigid->AddIgnoreTag("Boulder");
			pRigid->AddIgnoreTag("Maita");
			pRigid->AddIgnoreTag("ZenChan");

			//Bubble behaviour
			BoulderBehaviour* const pBehaviour = new BoulderBehaviour(pObj, pRigid, pAnimComp, isLookingLeft);
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
		const glm::vec3 myPos = m_pParent->GetPosition();

		//Find closest one
		float smallestDistance{};
		glm::vec3 playerPos{};
		bool found = false;
		for (size_t i{}; i < m_Targets.size(); ++i)
		{
			if (!m_Targets[i]->IsEnabled()) { continue; }

			const glm::vec3& pos = m_Targets[i]->GetPosition();
			const float newDist = glm::distance2(myPos, pos);
			if (!found || newDist < smallestDistance)
			{
				smallestDistance = newDist;
				playerPos = pos;
				found = true;
			}
		}
		if (!found) { return; }

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

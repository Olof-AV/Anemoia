#include "pch.h"
#include "ZenBehaviour.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "ColliderComponent.h"
#include "ItemBehaviour.h"

#include "Scene.h"
#include "BaseGameScene.h"
#include "ResourceManager.h"

#include "PlayerBehaviour.h"

#include "AIManager.h"

#include <algorithm>

ZenBehaviour::ZenBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp)
	: anemoia::BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_IsDead{ false }, m_CurrentState{ ZenState::run }
{
	m_InputDir = glm::vec2{};

	//Load texs
	const std::string startPath = "Enemies/ZenChan/";
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

	//Players
	m_Targets = pParent->GetParentScene()->GetObjectsWithTag("Player");

	//AI
	anemoia::BoundFunc func;
	func.func = std::bind(&ZenBehaviour::RunAI, this);
	func.pParent = pParent;
	func.pScene = pParent->GetParentScene();
	anemoia::AIManager::GetInstance()->RegisterFunction(func);
}

ZenBehaviour::~ZenBehaviour()
{
	//AI cleanup
	anemoia::BoundFunc func;
	func.func = std::bind(&ZenBehaviour::RunAI, this);
	func.pParent = GetParent();
	func.pScene = func.pParent->GetParentScene();
	anemoia::AIManager::GetInstance()->UnRegisterFunction(func);
}

void ZenBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void ZenBehaviour::Update(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case ZenState::run:
		HandleMovement();

		break;

	case ZenState::bubble:
		HandleBubbleMov();

		m_BubbleBurstTimer += elapsedSec;
		if (m_BubbleBurstTimer > m_BubbleBurstTimerMax) { SetState(ZenState::run); }

		break;
	}
}

void ZenBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void ZenBehaviour::OnCollide(anemoia::GameObject* const pOther)
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
		SetState(ZenState::bubble);
	}
}

ZenState ZenBehaviour::GetState() const
{
	return m_CurrentState;
}

void ZenBehaviour::SetState(ZenState newState)
{
	//Machine on new state
	switch (newState)
	{
	case ZenState::run:
		m_pTexComp->SetTexture(m_pTexRun);

		break;

	case ZenState::bubble:
		m_pTexComp->SetTexture(m_pTexBubble);
		m_BubbleBurstTimer = 0.f;

		break;
	}

	//Don't forget to change
	m_CurrentState = newState;
}

void ZenBehaviour::HandleMovement()
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
}

void ZenBehaviour::HandleBubbleMov()
{
	//Just floats up a little + reset input
	m_pRigid->SetVelocity(glm::vec2(0.f, m_FloatRate));
	m_InputDir = glm::vec2();
}

void ZenBehaviour::PlayerTouch(anemoia::GameObject* const pOther)
{
	//If touched while bubbled, this ZenChan dies
	if (m_CurrentState == ZenState::bubble)
	{
		if (!m_pParent->GetMarkForDelete())
		{
			//Spawn watermelon
			{
				//Obj
				anemoia::GameObject* const pObj = new anemoia::GameObject(m_pParent->GetParentScene());
				pObj->SetPosition(m_pParent->GetPosition());

				//Texture
				anemoia::Texture2D* const pTex = anemoia::ResourceManager::GetInstance()->LoadTexture("Items/Watermelon.png");
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
				ItemBehaviour* const pBehaviour = new ItemBehaviour(pObj, pRigid, pTexComp, anemoia::Events::PLAYER_OBTAIN_WATERMELON);
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

void ZenBehaviour::RunAI()
{
	//AI
	if (m_CurrentState != ZenState::bubble || m_Targets.empty())
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

			const glm::vec3 &pos = m_Targets[i]->GetPosition();
			const float newDist = glm::distance2(myPos, pos);
			if(!found || newDist < smallestDistance)
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
			}
		}
	}
	else
	{
		m_InputDir = glm::vec2();
	}
}

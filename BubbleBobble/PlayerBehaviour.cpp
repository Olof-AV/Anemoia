#include "pch.h"
#include "PlayerBehaviour.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

#include "RigidBodyComponent.h"
#include "TextureComponent.h"
#include "ColliderComponent.h"

#include "ResourceManager.h"

#include "Scene.h"

PlayerBehaviour::PlayerBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool isP2)
	: BaseComponent(pParent, anemoia::Transform()),
	m_pRigid{ pRigid }, m_pTexComp{ pTexComp },
	m_IsDead{false}
{
	m_InputDir = glm::vec2{};

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

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexIdle);

	//Params
	m_MovSpeed = 200.f;
	m_JumpForce = 600.f;
	m_IsInvincible = false;
	m_InvincibilityTimer = 0.f;
	m_InvincibilityTimerMax = 3.f;
}

void PlayerBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void PlayerBehaviour::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	if (m_IsDead)
	{
		m_IsInvincible = true;
		m_IsDead = false;
		m_pRigid->Move(glm::vec2(92.f, 620.f));
		m_pRigid->SetVelocity(glm::vec2(0.f, 0.f));
	}
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
		}
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 128.f));
	}
	else
	{
		m_pTexComp->SetColourMod(glm::vec4(255.f, 255.f, 255.f, 255.f));
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
		Die();
	}
}

void PlayerBehaviour::Die()
{
	if (!m_IsDead && !m_IsInvincible)
	{
		std::cout << "Die here";
		m_IsDead = true;

		m_pParent->Notify(anemoia::Events::PLAYER_DEATH);
	}
}

void PlayerBehaviour::ShootBubble()
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
	anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pObj, pColl);
	pObj->AddComponent(pRigid);
	pRigid->AddIgnoreTag("Treasure");

	//Tag
	pObj->AddTag("Bubble");

	//Add to scene
	pScene->AddChild(pObj);
}
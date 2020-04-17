#include "pch.h"
#include "PlayerBehaviour.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"

#include "ResourceManager.h"

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
		XINPUT_GAMEPAD_A, ((isP2) ? VK_NUMPAD5 : 'Z'), anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.y += -1.f;
	}));

	const std::string startPath = ((isP2) ? "Player/Bobby/" : "Player/Bubby/");

	//Load texs
	m_pTexIdle = anemoia::ResourceManager::GetInstance()->LoadTexture(startPath + "Idle.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexIdle);
}

void PlayerBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void PlayerBehaviour::Update(float elapsedSec)
{
	if (m_IsDead)
	{
		m_IsDead = false;
		m_pRigid->Move(glm::vec2(92.f, 620.f));
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
		glm::vec3 pos = GetParent()->GetPosition();
		const glm::vec2 newPos(pos.x + m_InputDir.x * 200.f * elapsedSec, pos.y);
		m_pRigid->Move(newPos);

		//Jump if touching floor
		if (m_pRigid->IsTouchingFloor())
		{
			m_pRigid->AddVelocity(glm::vec2(0.f, m_InputDir.y * 600.f));
		}

		//Reset
		m_InputDir = glm::vec2{};
	}
}

void PlayerBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void PlayerBehaviour::OnCollide(anemoia::GameObject* const pOther)
{
	BaseComponent::OnCollide(pOther);

	if (!m_IsDead)
	{
		if (pOther->HasTag("ZenChan"))
		{
			std::cout << "Die here";
			m_IsDead = true;

			m_pParent->Notify(anemoia::Events::PLAYER_DEATH);
		}
	}
}

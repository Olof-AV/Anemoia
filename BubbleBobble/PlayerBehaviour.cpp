#include "pch.h"
#include "PlayerBehaviour.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "TextureComponent.h"

#include "ResourceManager.h"

PlayerBehaviour::PlayerBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp)
	: BaseComponent(pParent, anemoia::Transform()), m_pRigid{ pRigid }, m_pTexComp{ pTexComp }
{
	m_InputDir = glm::vec2{};

	//Setup input
	anemoia::InputManager* const pInput = anemoia::InputManager::GetInstance();
	pInput->RegisterCommand(new anemoia::Command("MoveLeft", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x = -1.f;
	}));
	pInput->RegisterCommand(new anemoia::Command("MoveRight", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.x = 1.f;
	}));
	pInput->RegisterCommand(new anemoia::Command("Jump", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_A, 'Z', anemoia::ButtonState::Hold, [this]()
	{
		m_InputDir.y = -1.f;
	}));

	//Load texs
	m_pTexIdle = anemoia::ResourceManager::GetInstance()->LoadTexture("Player/Bubby/Idle.png");

	//Set tex for now
	m_pTexComp->SetTexture(m_pTexIdle);
}

void PlayerBehaviour::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void PlayerBehaviour::Update(float elapsedSec)
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

void PlayerBehaviour::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}
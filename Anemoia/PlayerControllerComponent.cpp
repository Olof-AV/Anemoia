#include "AnemoiaPCH.h"
#include "PlayerControllerComponent.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

anemoia::PlayerControllerComponent::PlayerControllerComponent(GameObject* const pParent, RigidBodyComponent* const pRigid)
	: BaseComponent(pParent, Transform()), m_pRigid{pRigid}
{
	m_InputDir = glm::vec2{};

	//Setup input
	InputManager* const pInput = InputManager::GetInstance();
	pInput->RegisterCommand(new Command("MoveLeft", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, ButtonState::Hold, [this]()
	{
		m_InputDir.x = -1.f;
	}));
	pInput->RegisterCommand(new Command("MoveRight", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, ButtonState::Hold, [this]()
	{
		m_InputDir.x = 1.f;
	}));
	pInput->RegisterCommand(new Command("Jump", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_A, 'Z', ButtonState::Down, [this]()
	{
		m_InputDir.y = -1.f;
	}));
}

void anemoia::PlayerControllerComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void anemoia::PlayerControllerComponent::Update(float elapsedSec)
{
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

void anemoia::PlayerControllerComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}
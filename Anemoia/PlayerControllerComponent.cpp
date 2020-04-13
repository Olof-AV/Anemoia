#include "AnemoiaPCH.h"
#include "PlayerControllerComponent.h"

#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

anemoia::PlayerControllerComponent::PlayerControllerComponent(GameObject* const pParent, const Transform& transform)
	: BaseComponent(pParent, transform)
{
	m_InputDir = glm::vec2{};

	InputManager* const pInput = InputManager::GetInstance();
	pInput->RegisterCommand(new Command("MoveLeft", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_LEFT, 'A', ButtonState::Hold, [this]()
	{
		m_InputDir.x = -1.f;
	}));

	pInput->RegisterCommand(new Command("MoveRight", pParent->GetParentScene(), 0, XINPUT_GAMEPAD_DPAD_RIGHT, 'D', ButtonState::Hold, [this]()
	{
		m_InputDir.x = 1.f;
	}));
}

void anemoia::PlayerControllerComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);

	glm::vec3 pos = GetParent()->GetPosition();
	GetParent()->SetPosition(pos + glm::vec3(m_InputDir * 100.f, 0.f) * timeStep);

	m_InputDir = glm::vec2{};
}

void anemoia::PlayerControllerComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::PlayerControllerComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::PlayerControllerComponent::Move(float timeStep, const glm::vec2& dir)
{
	UNREFERENCED_PARAMETER(timeStep);
	UNREFERENCED_PARAMETER(dir);
}

#include "AnemoiaPCH.h"
#include "InputManager.h"
#include <SDL.h>

#include <algorithm>
#include "Command.h"
#include "SceneManager.h"

anemoia::InputManager::InputManager()
{
	//Keyboard states
	m_KeyboardState = new BYTE[256];
	m_PreviousKeyboardState = new BYTE[256];
	ZeroMemory(m_KeyboardState, sizeof(BYTE) * 256);
	ZeroMemory(m_PreviousKeyboardState, sizeof(BYTE) * 256);

	//Clear commands
	m_Commands.clear();
}

anemoia::InputManager::~InputManager()
{
	//Delete states
	delete[] m_KeyboardState;
	delete[] m_PreviousKeyboardState;

	//Delete all stored commands
	std::for_each(m_Commands.cbegin(), m_Commands.cend(), [](Command* const pCommand)
	{
		delete pCommand;
	});
}

bool anemoia::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}

	//Update old states (4 controllers max)
	for (int i{}; i < XUSER_MAX_COUNT; ++i)
	{
		m_PadPreviousState[i] = m_PadInputState[i];
	}
	memcpy(m_PreviousKeyboardState, m_KeyboardState, sizeof(BYTE) * 256);

	//Read new state (4 controllers max)
	for (int i{}; i < XUSER_MAX_COUNT; ++i)
	{
		ZeroMemory(&m_PadInputState[i], sizeof(XINPUT_STATE));
		XInputGetState(i, &m_PadInputState[i]);
	}

	//Get new states, the GetKeyState is required for whatever reason
	ZeroMemory(m_KeyboardState, sizeof(BYTE) * 256);
	GetKeyState(0);
	const bool got = GetKeyboardState(m_KeyboardState);

	//If successful
	if (got)
	{
		SceneManager* const pSceneManager = SceneManager::GetInstance();

		//Get all active commands and return them
		std::for_each(m_Commands.cbegin(), m_Commands.cend(), [this, pSceneManager](Command* const pCommand)
		{
			//A command can only funtion if the scene it's bound to is currently active
			if (pSceneManager->IsSceneActive(pCommand->GetBoundScene()))
			{
				const ButtonState state = pCommand->GetButtonState();

				switch (state)
				{
				case ButtonState::Down:
					if (IsButtonDown(pCommand))
					{
						pCommand->Execute();
					}
					break;

				case ButtonState::Hold:
					if (IsHeld(pCommand))
					{
						pCommand->Execute();
					}
					break;

				case ButtonState::Up:
					if (IsButtonUp(pCommand))
					{
						pCommand->Execute();
					}
					break;
				}
			}
		});
	}

	//Did not quit
	return true;
}

void anemoia::InputManager::RegisterCommand(Command* const pCommand)
{
	//Will be managed by the input manager from now on
	m_Commands.push_back(pCommand);
}

anemoia::Command* anemoia::InputManager::UnregisterCommand(Command* const pCommand)
{
	//Remove from managed commands
	const std::vector<Command*>::const_iterator cIt = std::remove(m_Commands.begin(), m_Commands.end(), pCommand);
	if (cIt != m_Commands.cend())
	{
		m_Commands.erase(cIt);
		return pCommand;
	}

	return nullptr;
}

anemoia::Command* const anemoia::InputManager::GetCommand(const std::string& name) const
{
	//Look for command
	std::vector<Command*>::const_iterator cIt = std::find_if(m_Commands.cbegin(), m_Commands.cend(), [&name](Command* const pCom)
	{
		return pCom->GetName().compare(name) == 0;
	});

	//Return found/not found
	if (cIt != m_Commands.cend())
	{
		return *cIt;
	}
	return nullptr;
}

XINPUT_STATE anemoia::InputManager::GetControllerState(DWORD userIndex)
{
	return m_PadInputState[userIndex];
}

void anemoia::InputManager::SetControllerState(DWORD userIndex, WORD leftMotor, WORD rightMotor)
{
	//Create vibration state according to input data
	XINPUT_VIBRATION vibration{};
	vibration.wLeftMotorSpeed = leftMotor;
	vibration.wRightMotorSpeed = rightMotor;

	//Set vibration
	XInputSetState(userIndex, &vibration);
}

float anemoia::InputManager::GetTriggerAxis(DWORD userIndex, bool rightTrigger) const
{
	const BYTE axis = (rightTrigger) ? m_PadInputState[userIndex].Gamepad.bLeftTrigger : m_PadInputState[userIndex].Gamepad.bLeftTrigger;
	return (float)axis / 255.f;
}

float anemoia::InputManager::GetStickAxis(DWORD userIndex, bool yAxis, bool rightStick) const
{
	//Basically gets you the value of a stick between -1.f and 1.f
	const XINPUT_GAMEPAD pad = m_PadInputState[userIndex].Gamepad;
	const SHORT value = (rightStick) ? (yAxis) ? pad.sThumbRY : pad.sThumbRX : (yAxis) ? pad.sThumbLY : pad.sThumbLX;
	return ((float((int)value + 32767) / 65'534.f) * 2.f) - 1.f;
}

bool anemoia::InputManager::IsHeld(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();
	const int controllerID = pCommand->GetControllerID();

	//Get whether the condition should be positive or not
	const bool padStatus = (m_PadInputState[controllerID].Gamepad.wButtons & padKey);
	const bool keyboardStatus = m_KeyboardState[key] & 0xF0;

	//Return
	return padStatus || keyboardStatus;
}

bool anemoia::InputManager::IsButtonDown(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();
	const int controllerID = pCommand->GetControllerID();

	//Get whether the condition should be positive or not
	const bool padStatus = (m_PadInputState[controllerID].Gamepad.wButtons & padKey && !(m_PadPreviousState[controllerID].Gamepad.wButtons & padKey));
	const bool keyboardStatus = (m_KeyboardState[key] & 0xF0) && !(m_PreviousKeyboardState[key] & 0xF0);

	//Return
	return padStatus || keyboardStatus;
}

bool anemoia::InputManager::IsButtonUp(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();
	const int controllerID = pCommand->GetControllerID();

	//Get whether the condition should be positive or not
	const bool padStatus = (!(m_PadInputState[controllerID].Gamepad.wButtons & padKey) && (m_PadPreviousState[controllerID].Gamepad.wButtons & padKey));
	const bool keyboardStatus = !(m_KeyboardState[key] & 0xF0) && (m_PreviousKeyboardState[key] & 0xF0);

	//Return
	return padStatus || keyboardStatus;
}

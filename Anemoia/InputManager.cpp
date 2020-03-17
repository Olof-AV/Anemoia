#include "AnemoiaPCH.h"
#include "InputManager.h"
#include <SDL.h>

#include <algorithm>
#include "Command.h"

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

	//Update old
	m_PadPreviousState = m_PadInputState;
	memcpy(m_PreviousKeyboardState, m_KeyboardState, sizeof(BYTE) * 256);

	//Read new state
	ZeroMemory(&m_PadInputState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_PadInputState);

	//Get new states, the GetKeyState is required for whatever eason
	ZeroMemory(m_KeyboardState, sizeof(BYTE) * 256);
	GetKeyState(0);
	const bool got = GetKeyboardState(m_KeyboardState);

	//If successful
	if (got)
	{
		//Get all active commands and return them
		std::for_each(m_Commands.cbegin(), m_Commands.cend(), [this](Command* const pCommand)
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
		});
	}

	//Did not quit
	return true;
}

void anemoia::InputManager::RegisterCommand(Command* const pCommand)
{
	//Will be manged by the input manager from now on
	m_Commands.push_back(pCommand);
}

bool anemoia::InputManager::IsHeld(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();

	//Get whether the condition should be positive or not
	const bool padStatus = (m_PadInputState.Gamepad.wButtons & padKey);
	const bool keyboardStatus = m_KeyboardState[key] & 0xF0;

	//Return
	return padStatus || keyboardStatus;
}

bool anemoia::InputManager::IsButtonDown(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();

	//Get whether the condition should be positive or not
	const bool padStatus = (m_PadInputState.Gamepad.wButtons & padKey && !(m_PadPreviousState.Gamepad.wButtons & padKey));
	const bool keyboardStatus = (m_KeyboardState[key] & 0xF0) && !(m_PreviousKeyboardState[key] & 0xF0);

	//Return
	return padStatus || keyboardStatus;
}

bool anemoia::InputManager::IsButtonUp(Command* const pCommand) const
{
	//Get keys associated
	const int key = pCommand->GetKeyboardButton();
	const int padKey = pCommand->GetPadButton();

	//Get whether the condition should be positive or not
	const bool padStatus = (!(m_PadInputState.Gamepad.wButtons & padKey) && (m_PadPreviousState.Gamepad.wButtons & padKey));
	const bool keyboardStatus = !(m_KeyboardState[key] & 0xF0) && (m_PreviousKeyboardState[key] & 0xF0);

	//Return
	return padStatus || keyboardStatus;
}

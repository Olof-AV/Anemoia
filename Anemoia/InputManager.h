#pragma once
#include <XInput.h>
#include "Singleton.h"

namespace anemoia
{
	class Command;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void RegisterCommand(Command* const pCommand);
		Command* UnregisterCommand(Command* const pCommand);
		Command* const GetCommand(const std::string &name) const;

		//State
		XINPUT_STATE GetControllerState(DWORD userIndex);
		void SetControllerState(DWORD userIndex, float leftMotor, float rightMotor);

		//Axes
		float GetTriggerAxis(DWORD userIndex, bool rightTrigger) const;
		float GetStickAxis(DWORD userIndex, bool yAxis, bool rightStick) const;

	private:
		friend class Singleton<InputManager>;

		InputManager();
		virtual ~InputManager();

		bool IsHeld(Command* const pCommand) const;
		bool IsButtonDown(Command* const pCommand) const;
		bool IsButtonUp(Command* const pCommand) const;

		std::vector<class Command*> m_Commands;

		//Gamepad
		XINPUT_STATE m_PadInputState[XUSER_MAX_COUNT];
		XINPUT_STATE m_PadPreviousState[XUSER_MAX_COUNT];

		//Keyboard
		BYTE* m_KeyboardState;
		BYTE* m_PreviousKeyboardState;
	};
}
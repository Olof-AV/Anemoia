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

	private:
		friend class Singleton<InputManager>;

		InputManager();
		virtual ~InputManager();

		bool IsHeld(Command* const pCommand) const;
		bool IsButtonDown(Command* const pCommand) const;
		bool IsButtonUp(Command* const pCommand) const;

		std::vector<class Command*> m_Commands;

		//Gamepad
		XINPUT_STATE m_PadInputState{};
		XINPUT_STATE m_PadPreviousState{};

		//Keyboard
		BYTE* m_KeyboardState;
		BYTE* m_PreviousKeyboardState;
	};
}
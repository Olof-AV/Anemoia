#include "AnemoiaPCH.h"
#include "Command.h"

anemoia::Command::Command(int padButton, int keyboardButton, ButtonState buttonState, CommandCallback callback)
	: m_PadButton{ padButton }, m_KeyboardButton{ keyboardButton }, m_State{ buttonState }, m_Callback{callback}
{
}

void anemoia::Command::Execute()
{
	//If valid, call
	if (m_Callback)
	{
		m_Callback();
	}
}

#pragma region GettersSetters
anemoia::ButtonState anemoia::Command::GetButtonState() const
{
	return m_State;
}

void anemoia::Command::SetButtonState(ButtonState buttonState)
{
	m_State = buttonState;
}

int anemoia::Command::GetPadButton() const
{
	return m_PadButton;
}

void anemoia::Command::SetPadButton(int padButton)
{
	m_PadButton = padButton;
}

int anemoia::Command::GetKeyboardButton() const
{
	return m_KeyboardButton;
}

void anemoia::Command::SetKeyboardButton(char keyboardButton)
{
	m_KeyboardButton = keyboardButton;
}

anemoia::CommandCallback anemoia::Command::GetCommandCallback() const
{
	return m_Callback;
}

void anemoia::Command::SetCommandCallback(CommandCallback callback)
{
	m_Callback = callback;
}
#pragma endregion GettersSetters

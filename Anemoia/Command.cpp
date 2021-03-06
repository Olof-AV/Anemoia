#include "AnemoiaPCH.h"
#include "Command.h"

anemoia::Command::Command(const std::string& commandName, Scene* const boundScene, int controllerId, int padButton, int keyboardButton, ButtonState buttonState, CommandCallback callback)
	: m_CommandName{ commandName },
	m_ControllerID {controllerId},
	m_PadButton{ padButton }, m_KeyboardButton{ keyboardButton },
	m_State{ buttonState }, m_Callback{ callback },
	m_BoundScene{boundScene}
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

void anemoia::Command::SetKeyboardButton(int keyboardButton)
{
	m_KeyboardButton = keyboardButton;
}

int anemoia::Command::GetControllerID() const
{
	return m_ControllerID;
}

void anemoia::Command::SetControllerID(int controllerId)
{
	m_ControllerID = controllerId;
}

anemoia::CommandCallback anemoia::Command::GetCommandCallback() const
{
	return m_Callback;
}

void anemoia::Command::SetCommandCallback(CommandCallback callback)
{
	m_Callback = callback;
}
const std::string& anemoia::Command::GetName() const
{
	return m_CommandName;
}
anemoia::Scene* const anemoia::Command::GetBoundScene() const
{
	return m_BoundScene;
}
#pragma endregion GettersSetters

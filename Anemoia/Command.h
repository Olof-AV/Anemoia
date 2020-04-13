#pragma once

#include <functional>

namespace anemoia
{
	class Scene;

	//Callback for commands
	using CommandCallback = std::function<void()>;

	//Enum for states a button can be in
	enum class ButtonState
	{
		Hold,
		Down,
		Up
	};

	class Command final
	{
	public:
		Command(const std::string &commandName, Scene* const boundScene, int controllerId, int padButton, int keyboardButton, ButtonState buttonState, CommandCallback callback);
		~Command() = default;

		void Execute();

#pragma region GettersSetters
		ButtonState GetButtonState() const;
		void SetButtonState(ButtonState buttonState);

		int GetPadButton() const;
		void SetPadButton(int padButton);

		int GetKeyboardButton() const;
		void SetKeyboardButton(int keyboardButton);

		int GetControllerID() const;
		void SetControllerID(int controllerId);

		CommandCallback GetCommandCallback() const;
		void SetCommandCallback(CommandCallback callback);

		const std::string& GetName() const;

		Scene* const GetBoundScene() const;
#pragma endregion GettersSetters

	private:
		CommandCallback m_Callback;
		ButtonState m_State;

		std::string m_CommandName;

		Scene* m_BoundScene;

		int m_PadButton;
		int m_KeyboardButton;
		int m_ControllerID;
	};
}
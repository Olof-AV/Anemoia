#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace anemoia
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		TextComponent(GameObject* const pParent, const Transform& transform,
			const std::string& text, Font* const pFont, const SDL_Colour& textColour = SDL_Colour{ 255, 255, 255 });

		virtual ~TextComponent();

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void Render() const override;

		//Functions
		void SetText(const std::string& text);

	private:
		//Extra
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture;

		SDL_Colour m_TextColour;
	};
}

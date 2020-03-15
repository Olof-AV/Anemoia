#include "AnemoiaPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include "Locator.h"

#include "GameObject.h"

TextComponent::TextComponent(GameObject* const pParent, const Transform& transform,
	const std::string& text, Font* const pFont, const SDL_Colour& textColour)
	: BaseComponent(pParent, transform),
	m_NeedsUpdate(true), m_Text(text), m_pFont(pFont), m_TextColour{textColour},
	m_pTexture(nullptr)
{

}

TextComponent::~TextComponent()
{
	//Delete texture but not font, font is managed by resource manager
	delete m_pTexture;
}

void TextComponent::FixedUpdate(float timeStep)
{
	//Mark as unreferenced for now
	UNREFERENCED_PARAMETER(timeStep);

	//If needs update, create new texture
	if (m_NeedsUpdate)
	{
		SDL_Surface* const pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_TextColour);
		if (!pSurface)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		const float x = (float)pSurface->w;
		const float y = (float)pSurface->h;

		SDL_Texture* const pTexture = SDL_CreateTextureFromSurface(Locator::GetRenderer(), pSurface);
		if (!pTexture)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		//AFter you are done with a surface, free it
		SDL_FreeSurface(pSurface);

		//Create new texture 2D (managed by self)
		delete m_pTexture;
		m_pTexture = new Texture2D(pTexture, glm::vec2(x, y));
		m_NeedsUpdate = false;
	}
}

void TextComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void TextComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		//Calculate params
		const glm::vec2 size = m_pTexture->GetDimensions() * m_Transform.GetScale();
		const glm::vec2 pivotOffset = (m_Transform.GetPivot() * size);
		const glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;

		//Render text texture
		Renderer::GetInstance()->RenderTexture(m_pTexture, finalPos.x, finalPos.y, size.x, size.y, m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip());
	}
}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
#include "AnemoiaPCH.h"
#include "TextureComponent.h"

#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

#include "Locator.h"

anemoia::TextureComponent::TextureComponent(GameObject* const pParent, const Transform& transform,
	Texture2D* const pTexture, const glm::vec4& colour)
	: BaseComponent(pParent, transform), m_pTexture(pTexture), m_ColourMod{colour}
{
}

void anemoia::TextureComponent::FixedUpdate(float /*timeStep*/)
{

}

void anemoia::TextureComponent::Update(float /*elapsedSec*/)
{

}

void anemoia::TextureComponent::LateUpdate(float /*elapsedSec*/)
{

}

void anemoia::TextureComponent::Render() const
{
	if (m_pTexture)
	{
		//Calculate params
		const glm::vec2 size = m_pTexture->GetDimensions() * m_Transform.GetScale();
		const glm::vec2 pivotOffset = (m_Transform.GetPivot() * size);
		const glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;

		//Obtain global blend mode and set that as texture blend mode
#pragma warning(suppress:26812)
		SDL_BlendMode blendMode;
		SDL_GetRenderDrawBlendMode(Locator::GetRenderer(), &blendMode);
		SDL_SetTextureBlendMode(m_pTexture->GetSDLTexture(), blendMode);

		//Set colour according to mod
		SDL_SetTextureColorMod(m_pTexture->GetSDLTexture(), (Uint8)m_ColourMod.r, (Uint8)m_ColourMod.g, (Uint8)m_ColourMod.b);
		SDL_SetTextureAlphaMod(m_pTexture->GetSDLTexture(), (Uint8)m_ColourMod.a);

		//Render texture
		Renderer::GetInstance()->RenderTexture(m_pTexture, finalPos.x, finalPos.y, size.x, size.y,
			m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip());
	}
}

anemoia::Texture2D* const anemoia::TextureComponent::GetTexture() const
{
	return m_pTexture;
}

void anemoia::TextureComponent::SetTexture(Texture2D* const pTexture)
{
	m_pTexture = pTexture;
}

const glm::vec4& anemoia::TextureComponent::GetColourMod() const
{
	return m_ColourMod;
}

void anemoia::TextureComponent::SetColourMod(const glm::vec4& colour)
{
	m_ColourMod = colour;
}

float anemoia::TextureComponent::GetAlpha() const
{
	return m_ColourMod.a;
}

void anemoia::TextureComponent::SetAlpha(float value)
{
	m_ColourMod.a = value;
}

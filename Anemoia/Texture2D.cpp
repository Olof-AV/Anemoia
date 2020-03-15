#include "AnemoiaPCH.h"
#include "Texture2D.h"
#include <SDL.h>

Texture2D::Texture2D(SDL_Texture* const pTexture, const glm::vec2 dimensions)
	: Resource{}, m_pTexture{ pTexture }, m_Dimensions(dimensions)
{

}

Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

const glm::vec2& Texture2D::GetDimensions() const
{
	return m_Dimensions;
}
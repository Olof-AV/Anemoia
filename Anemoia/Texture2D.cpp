#include "AnemoiaPCH.h"
#include "Texture2D.h"
#include <SDL.h>

using namespace anemoia;

anemoia::Texture2D::Texture2D(SDL_Texture* const pTexture, const glm::vec2 dimensions)
	: Resource{}, m_pTexture{ pTexture }, m_Dimensions(dimensions)
{

}

anemoia::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* anemoia::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

const glm::vec2& anemoia::Texture2D::GetDimensions() const
{
	return m_Dimensions;
}
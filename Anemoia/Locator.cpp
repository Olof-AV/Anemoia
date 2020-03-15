#include "AnemoiaPCH.h"
#include "Locator.h"

SDL_Window* anemoia::Locator::m_pWindow = nullptr;
SDL_Renderer* anemoia::Locator::m_pRenderer = nullptr;

SDL_Window* const anemoia::Locator::GetWindow()
{
	return m_pWindow;
}

SDL_Renderer* const anemoia::Locator::GetRenderer()
{
	return m_pRenderer;
}

void anemoia::Locator::SetSDL_Window(SDL_Window* const pWindow)
{
	m_pWindow = pWindow;
}

void anemoia::Locator::SetSDL_Renderer(SDL_Renderer* const pRenderer)
{
	m_pRenderer = pRenderer;
}

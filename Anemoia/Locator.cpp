#include "AnemoiaPCH.h"
#include "Locator.h"

SDL_Window* Locator::m_pWindow = nullptr;
SDL_Renderer* Locator::m_pRenderer = nullptr;

SDL_Window* const Locator::GetWindow()
{
	return m_pWindow;
}

SDL_Renderer* const Locator::GetRenderer()
{
	return m_pRenderer;
}

void Locator::SetSDL_Window(SDL_Window* const pWindow)
{
	m_pWindow = pWindow;
}

void Locator::SetSDL_Renderer(SDL_Renderer* const pRenderer)
{
	m_pRenderer = pRenderer;
}

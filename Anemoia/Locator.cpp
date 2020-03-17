#include "AnemoiaPCH.h"
#include "Locator.h"

SDL_Window* anemoia::Locator::m_pWindow = nullptr;
SDL_Renderer* anemoia::Locator::m_pRenderer = nullptr;
anemoia::InputManager* anemoia::Locator::m_pInputManager = nullptr;

SDL_Window* const anemoia::Locator::GetWindow()
{
	return m_pWindow;
}

SDL_Renderer* const anemoia::Locator::GetRenderer()
{
	return m_pRenderer;
}

anemoia::InputManager* const anemoia::Locator::GetInputManager()
{
	return m_pInputManager;
}

void anemoia::Locator::SetSDL_Window(SDL_Window* const pWindow)
{
	m_pWindow = pWindow;
}

void anemoia::Locator::SetSDL_Renderer(SDL_Renderer* const pRenderer)
{
	m_pRenderer = pRenderer;
}

void anemoia::Locator::SetInputManager(InputManager* const pInputManager)
{
	m_pInputManager = pInputManager;
}

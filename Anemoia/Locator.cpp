#include "AnemoiaPCH.h"
#include "Locator.h"

SDL_Window* anemoia::Locator::m_pWindow = nullptr;
SDL_Renderer* anemoia::Locator::m_pRenderer = nullptr;
anemoia::InputManager* anemoia::Locator::m_pInputManager = nullptr;
anemoia::Engine* anemoia::Locator::m_pEngine = nullptr;

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

anemoia::Engine* const anemoia::Locator::GetEngine()
{
	return m_pEngine;
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

void anemoia::Locator::SetEngine(Engine* const pEngine)
{
	m_pEngine = pEngine;
}

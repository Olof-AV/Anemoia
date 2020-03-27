#pragma once
// http://gameprogrammingpatterns.com/service-locator.html
// Provide a global point of access to a service without 
// coupling users to the concrete class that implements it.

#include "Anemoia.h"
#include "Renderer.h"
#include "InputManager.h"

struct SDL_Window;
struct SDL_Renderer;

namespace anemoia
{
	class Locator
	{
		friend Engine;
		friend Renderer;
		friend InputManager;

	public:
		static SDL_Window* const GetWindow();
		static SDL_Renderer* const GetRenderer();
		static InputManager* const GetInputManager();
		static Engine* const GetEngine();

	private:
		void static SetSDL_Window(SDL_Window* const pWindow);
		void static SetSDL_Renderer(SDL_Renderer* const pRenderer);
		void static SetInputManager(InputManager* const pInputManager);
		void static SetEngine(Engine* const pEngine);

		static SDL_Window* m_pWindow;
		static SDL_Renderer* m_pRenderer;
		static InputManager* m_pInputManager;
		static Engine* m_pEngine;
	};
}
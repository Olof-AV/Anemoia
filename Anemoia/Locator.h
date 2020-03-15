#pragma once
// http://gameprogrammingpatterns.com/service-locator.html
// Provide a global point of access to a service without 
// coupling users to the concrete class that implements it.

#include "Anemoia.h"
#include "Renderer.h"

struct SDL_Window;
struct SDL_Renderer;

namespace anemoia
{
	class Locator
	{
		friend Engine;
		friend Renderer;

	public:
		static SDL_Window* const GetWindow();
		static SDL_Renderer* const GetRenderer();

	private:
		void static SetSDL_Window(SDL_Window* const pWindow);
		void static SetSDL_Renderer(SDL_Renderer* const pRenderer);

		static SDL_Window* m_pWindow;
		static SDL_Renderer* m_pRenderer;
	};
}
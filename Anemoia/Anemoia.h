#pragma once
struct SDL_Window;

namespace anemoia
{
	class Engine
	{
	public:
		void Initialise();
		void LoadGame() const;
		void Cleanup();
		void Run();

	private:
		static const float m_TimeStep; //0.016 for 60 fps, 0.033 for 30 fps
		static const float m_MaxElapsedSec; //Prevent stupid stuff due to breakpoints
		SDL_Window* m_pWindow{};
	};
}
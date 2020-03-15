#include "AnemoiaPCH.h"
#include "Anemoia.h"

#include <chrono>
#include <thread>

#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <SDL.h>
#include "TextComponent.h"
#include "GameObject.h"

#include "Scene.h"
#include "FrameCounterScene.h"

#include "Locator.h"

const float Anemoia::Engine::m_TimeStep = 0.016f;

void Anemoia::Engine::Initialise()
{
	//Try to initiate video
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Try to create window
	m_pWindow = SDL_CreateWindow(
		"Programming 4 Assignment - AVIRON-VIOLET Olof",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	//Throw if there is an errror
	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//Setup locator
	Locator::SetSDL_Window(m_pWindow);

	//Initialise renderer with window
	Renderer::GetInstance()->Init(m_pWindow);
}

/**
 * Code constructing the scene world starts here
 */
void Anemoia::Engine::LoadGame() const
{
	Scene* const pScene = new FrameCounterScene();
	SceneManager::GetInstance()->AddScene(pScene);

	//Make scene

	/*auto go = std::make_shared<GameObject>();
	go->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<TextComponent>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);*/
}

void Anemoia::Engine::Cleanup()
{
	//Destroy all managers
	Renderer::DestroyInstance();
	SceneManager::DestroyInstance();
	InputManager::DestroyInstance();
	ResourceManager::DestroyInstance();

	//Destroy window instance
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void Anemoia::Engine::Run()
{
	//Initialise the game
	Initialise();

	//Initialise managers
	Renderer* const pRenderer = Renderer::GetInstance();
	SceneManager* const pSceneManager = SceneManager::GetInstance();
	InputManager* const pInput = InputManager::GetInstance();

	//Tell the resource manager where it can find the game data
	ResourceManager::GetInstance()->Init("../Data/");

	//Load the game
	LoadGame();

	//Initialise scenes
	pSceneManager->InitialiseScenes();

	//Game loop
	{
		//Initialise prev time
		std::chrono::time_point prevTime = std::chrono::steady_clock::now();
		float lag = 0.f;

		//Main game loop
		bool doContinue = true;
		while (doContinue)
		{
			//Get current time, and deduce deltaTime from it
			const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
			const float elapsedSec = std::chrono::duration<float>(currentTime - prevTime).count();
			lag += elapsedSec;

			//Update previous time
			prevTime = std::chrono::steady_clock::now();
			
			//Run our loop
			doContinue = pInput->ProcessInput();

			while (lag >= m_TimeStep)
			{
				pSceneManager->FixedUpdate(m_TimeStep);
				lag -= m_TimeStep;
			}

			//Update
			pSceneManager->Update(elapsedSec);

			//Render
			pRenderer->Render();
		}
	}

	//Game is closing
	Cleanup();
}

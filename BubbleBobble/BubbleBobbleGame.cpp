#include "pch.h"

#include "BubbleBobbleGame.h"
#include "Scene.h"
#include "FrameCounterScene.h"

#include "IntroScene.h"
#include "Level1.h"

#include <SDL.h>
#include <SDL_video.h>

void BubbleBobbleGame::LoadGame() const
{
	//Test
	anemoia::Scene* const pScene = new FrameCounterScene();
	anemoia::SceneManager::GetInstance()->AddScene(pScene);
	//anemoia::SceneManager::GetInstance()->SetActiveScene(pScene);

	//Game intro scene
	anemoia::Scene* const pIntro = new IntroScene();
	anemoia::SceneManager::GetInstance()->AddScene(pIntro);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pIntro);

	//Level 1 scene
	anemoia::Scene* const pLevel1 = new Level1();
	anemoia::SceneManager::GetInstance()->AddScene(pLevel1);

	//Some extra changes
	SDL_SetWindowTitle(m_pWindow, "Bubble Bobble - 2DAE01 - AVIRON-VIOLET Olof");
	//SDL_SetWindowFullscreen(m_pWindow, 1);
	SDL_SetWindowSize(m_pWindow, 256, 224);
}
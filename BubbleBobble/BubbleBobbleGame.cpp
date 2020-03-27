#include "pch.h"

#include "BubbleBobbleGame.h"
#include "Scene.h"
#include "FrameCounterScene.h"

#include <SDL.h>
#include <SDL_video.h>

void BubbleBobbleGame::LoadGame() const
{
	anemoia::Scene* const pScene = new FrameCounterScene();
	anemoia::SceneManager::GetInstance()->AddScene(pScene);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pScene);

	SDL_SetWindowTitle(m_pWindow, "Bubble Bobble - 2DAE01 - AVIRON-VIOLET Olof");
	SDL_SetWindowFullscreen(m_pWindow, 0);
}

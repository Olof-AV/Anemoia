#include "pch.h"

#include "BubbleBobbleGame.h"
#include "Scene.h"
#include "FrameCounterScene.h"

void BubbleBobbleGame::LoadGame() const
{
	anemoia::Scene* const pScene = new FrameCounterScene();
	anemoia::SceneManager::GetInstance()->AddScene(pScene);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pScene);
}

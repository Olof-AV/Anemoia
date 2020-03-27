#include "pch.h"

#include "BubbleBobbleGame.h"

#include "Scene.h"
#include "FrameCounterScene.h"
#include "IntroScene.h"
#include "Level1.h"

#include <SDL.h>
#include <SDL_video.h>

#include "Locator.h"

void BubbleBobbleGame::LoadGame() const
{
	//Test
	/*anemoia::Scene* const pScene = new FrameCounterScene();
	anemoia::SceneManager::GetInstance()->AddScene(pScene);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pScene);*/

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
	SDL_SetWindowSize(m_pWindow, 256 * 3, 224 * 3);
}

Gamemode BubbleBobbleGame::GetGamemode() const
{
	return m_CurrentMode;
}

int BubbleBobbleGame::GetScore(bool isP1) const
{
	return (isP1) ? m_ScoreP1 : m_ScoreP2;
}

bool BubbleBobbleGame::GetScoreChanged(bool isP1) const
{
	return (isP1) ? m_ScoreP1Changed : m_ScoreP2Changed;
}

int BubbleBobbleGame::GetLives(bool isP1) const
{
	return (isP1) ? m_LivesP1 : m_LivesP2;
}

void BubbleBobbleGame::AddScore(int change, bool isP1)
{
	(isP1) ? m_ScoreP1 += change : m_ScoreP2 += change;
	(isP1) ? m_ScoreP1Changed = true : m_ScoreP2Changed = true;
}

void BubbleBobbleGame::SetScoreChanged(bool isP1, bool value)
{
	(isP1) ? m_ScoreP1Changed = value : m_ScoreP2Changed = value;
}

void BubbleBobbleGame::SetLives(bool isP1, int value)
{
	(isP1) ? m_LivesP1 = value : m_LivesP2 = value;
}

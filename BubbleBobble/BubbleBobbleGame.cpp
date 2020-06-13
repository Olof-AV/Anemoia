#include "pch.h"

#include "BubbleBobbleGame.h"

#include "Scene.h"
#include "FrameCounterScene.h"
#include "IntroScene.h"
#include "BaseGameScene.h"
#include "StartScene.h"
#include "EndScene.h"
#include "FailScene.h"

#include <SDL.h>
#include <SDL_video.h>

#include "Locator.h"

#include "ResourceManager.h"
#include "Sound.h"

void BubbleBobbleGame::LoadGame()
{
	//Default params
	m_HiScore = 0;
	m_ScoreP1 = 0;
	m_ScoreP2 = 0;

	m_LivesP1 = 4;
	m_LivesP2 = 4;
	m_DeadP1 = false;
	m_DeadP2 = false;
	
	//Load stuff
	m_pSound_Collect = anemoia::ResourceManager::GetInstance()->LoadSound("Items/Pickup.wav");
	m_pSound_Music = anemoia::ResourceManager::GetInstance()->LoadSound("MainTheme.mp3");

	//Some extra changes
	SDL_SetWindowTitle(m_pWindow, "Bubble Bobble - 2DAE01 - AVIRON-VIOLET Olof");
	//SDL_SetWindowFullscreen(m_pWindow, 1);
	const int scale = 3;
	const int width = 256;
	const int height = 224;
	SDL_SetWindowSize(m_pWindow, width * scale, height * scale);

	//Test
	/*anemoia::Scene* const pScene = new FrameCounterScene();
	anemoia::SceneManager::GetInstance()->AddScene(pScene);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pScene);*/

	//Start screen, select gamemode + add end scene already
	anemoia::Scene* const pStart = new StartScene();
	anemoia::SceneManager::GetInstance()->AddScene(pStart);
	anemoia::SceneManager::GetInstance()->SetActiveScene(pStart);

	anemoia::Scene* const pEnd = new EndScene();
	anemoia::SceneManager::GetInstance()->AddScene(pEnd);

	anemoia::Scene* const pFail = new FailScene();
	anemoia::SceneManager::GetInstance()->AddScene(pFail);
}

Gamemode BubbleBobbleGame::GetGamemode() const
{
	return m_CurrentMode;
}

void BubbleBobbleGame::SetGamemode(Gamemode mode)
{
	m_CurrentMode = mode;
}

void BubbleBobbleGame::PlayMusic(bool value)
{
	if (value)
	{
		m_pSound_Music->Play(-1);
	}
	else
	{
		m_pSound_Music->FadeOut(2000);
	}
}

int BubbleBobbleGame::GetHiScore() const
{
	return m_HiScore;
}

int BubbleBobbleGame::GetScore(bool isP1) const
{
	return (isP1) ? m_ScoreP1 : m_ScoreP2;
}

int BubbleBobbleGame::GetLives(bool isP1) const
{
	return (isP1) ? m_LivesP1 : m_LivesP2;
}

void BubbleBobbleGame::AddScore(int change, bool isP1)
{
	//Change relevant score
	(isP1) ? m_ScoreP1 += change : m_ScoreP2 += change;

	//Hiscore changed?
	if (m_ScoreP1 > m_HiScore) { m_HiScore = m_ScoreP1; }
	if (m_ScoreP2 > m_HiScore) { m_HiScore = m_ScoreP2; }

	//Sound
	m_pSound_Collect->Play(0);
}

void BubbleBobbleGame::SetLives(bool isP1, int value)
{
	(isP1) ? m_LivesP1 = value : m_LivesP2 = value;
}

bool BubbleBobbleGame::NotifyPlayerDeath(bool isP1)
{
	const int amountLives = ((isP1) ? m_LivesP1 : m_LivesP2);
	if (amountLives > 0)
	{
		SetLives(isP1, amountLives - 1);
		return false;
	}
	else
	{
		switch (m_CurrentMode)
		{
		case Gamemode::singleplayer:
		{
			return true;
		}
			break;

		case Gamemode::multiplayer:
		{
			((isP1) ? m_DeadP1 : m_DeadP2) = true;

			if (m_DeadP1 && m_DeadP2)
			{
				return true;
			}
		}
			break;

		case Gamemode::versus:
		{
			((isP1) ? m_DeadP1 : m_DeadP2) = true;

			if (m_DeadP1)
			{
				return true;
			}
		}
			break;
		}
	}

	return false;
}

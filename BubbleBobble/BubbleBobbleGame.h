#pragma once
#include "Anemoia.h"
#include "Locator.h"

enum class Gamemode
{
	singleplayer,
	multiplayer,
	versus
};

namespace anemoia
{
	class Sound;
}

class BubbleBobbleGame final : public anemoia::Engine
{
public:
	virtual void LoadGame() override;

	//Getters/Setters
	Gamemode GetGamemode() const;
	void SetGamemode(Gamemode mode);

	//Music
	void PlayMusic(bool value);

	//Score--lives getters
	int GetHiScore() const;
	int GetScore(bool isP1) const;
	int GetLives(bool isP1) const;

	//Score--lives setters
	void AddScore(int change, bool isP1);
	void SetLives(bool isP1, int value);

	//Notify
	bool NotifyPlayerDeath(bool isP1);

private:
	//Mode
	Gamemode m_CurrentMode = Gamemode::singleplayer;

	//Data to keep between scenes
	int m_HiScore = 0;
	int m_ScoreP1 = 0;
	int m_ScoreP2 = 0;

	int m_LivesP1 = 4;
	int m_LivesP2 = 4;
	bool m_DeadP1 = false;
	bool m_DeadP2 = false;
	
	//Sound
	anemoia::Sound* m_pSound_Collect = nullptr;
	anemoia::Sound* m_pSound_Music = nullptr;
};
#pragma once
#include "Anemoia.h"
#include "Locator.h"

enum class Gamemode
{
	singleplayer,
	multiplayer,
	versus
};

class BubbleBobbleGame : public anemoia::Engine
{
public:
	virtual void LoadGame() const override;

	//Getters/Setters
	Gamemode GetGamemode() const;

	int GetHiScore() const;
	int GetScore(bool isP1) const;
	bool GetHiScoreChanged() const;
	bool GetScoreChanged(bool isP1) const;
	int GetLives(bool isP1) const;

	void AddScore(int change, bool isP1);
	void SetHiScoreChanged(bool value);
	void SetScoreChanged(bool isP1, bool value);
	void SetLives(bool isP1, int value);

	void NotifyPlayerDeath(bool isP1);

private:
	//Mode
	Gamemode m_CurrentMode;

	//Data to keep between scenes
	int m_HiScore = 123550;
	int m_ScoreP1 = 11110;
	int m_ScoreP2 = 1050;

	bool m_HiScoreChanged = false;
	bool m_ScoreP1Changed = false;
	bool m_ScoreP2Changed = false;

	int m_LivesP1 = 4;
	int m_LivesP2 = 4;
};
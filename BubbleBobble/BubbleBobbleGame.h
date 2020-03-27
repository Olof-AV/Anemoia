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

	int GetScore(bool isP1) const;
	bool GetScoreChanged(bool isP1) const;
	int GetLives(bool isP1) const;

	void AddScore(int change, bool isP1);
	void SetScoreChanged(bool isP1, bool value);
	void SetLives(bool isP1, int value);

private:
	//Mode
	Gamemode m_CurrentMode;

	//Data to keep between scenes
	int m_ScoreP1 = 0;
	int m_ScoreP2 = 0;

	bool m_ScoreP1Changed = true;
	bool m_ScoreP2Changed = true;

	int m_LivesP1 = 4;
	int m_LivesP2 = 4;
};
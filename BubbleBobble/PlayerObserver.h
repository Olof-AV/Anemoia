#pragma once
#include "Observer.h"

class BubbleBobbleGame;
class PlayerBehaviour;

class PlayerObserver final : public anemoia::Observer
{
public:
	PlayerObserver(PlayerBehaviour* const pBehaviour, bool isP1 = true);
	virtual ~PlayerObserver() = default;
	virtual void Notify(anemoia::Events event) override;

private:
	BubbleBobbleGame* m_pGame;
	PlayerBehaviour* const m_pBehaviour;
	bool m_IsP1;
};


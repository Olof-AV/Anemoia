#pragma once
#include "Observer.h"

class BubbleBobbleGame;

class PlayerObserver final : public anemoia::Observer
{
public:
	PlayerObserver(bool isP1 = true);
	virtual ~PlayerObserver() = default;
	virtual void Notify(anemoia::Events event) override;

private:
	BubbleBobbleGame* m_pGame;
	bool m_IsP1;
};


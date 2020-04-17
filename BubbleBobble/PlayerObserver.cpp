#include "pch.h"
#include "PlayerObserver.h"
#include "BubbleBobbleGame.h"
#include "Locator.h"

PlayerObserver::PlayerObserver(bool isP1)
	: m_IsP1{isP1}
{
	m_pGame = static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine());
}

void PlayerObserver::Notify(anemoia::Events event)
{
	switch (event)
	{
	case anemoia::Events::PLAYER_DEATH:

		m_pGame->NotifyPlayerDeath(m_IsP1);
		break;
	}
}

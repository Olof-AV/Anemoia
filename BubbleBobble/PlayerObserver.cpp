#include "pch.h"
#include "PlayerObserver.h"
#include "BubbleBobbleGame.h"
#include "Locator.h"

#include "SceneManager.h"
#include "BaseGameScene.h"

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
		static_cast<BaseGameScene*>(anemoia::SceneManager::GetInstance()->GetActiveScene())->NotifyPlayerDeath();
		break;

	case anemoia::Events::PLAYER_OBTAIN_WATERMELON:

		m_pGame->AddScore(100, m_IsP1);
		break;

	case anemoia::Events::PLAYER_OBTAIN_FRIES:

		m_pGame->AddScore(200, m_IsP1);
		break;
	}
}

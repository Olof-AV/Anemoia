#include "pch.h"
#include "MaitaObserver.h"
#include "MaitaBehaviour.h"

MaitaObserver::MaitaObserver(MaitaBehaviour* const pBehaviour)
	: m_pBehaviour(pBehaviour)
{
}

void MaitaObserver::Notify(anemoia::Events event)
{
	switch (event)
	{
	case anemoia::Events::PLAYER_DEATH:
		m_pBehaviour->CalmDown();

		break;

	case anemoia::Events::BUBBLE_TOUCH_P1:
		m_pBehaviour->GetBubbled(true);

		break;

	case anemoia::Events::BUBBLE_TOUCH_P2:
		m_pBehaviour->GetBubbled(false);

		break;

	default:
		break;
	}
}

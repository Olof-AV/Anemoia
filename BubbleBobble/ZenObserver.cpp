#include "pch.h"
#include "ZenObserver.h"

#include "ZenBehaviour.h"

ZenObserver::ZenObserver(ZenBehaviour* const pBehaviour)
	: m_pBehaviour(pBehaviour)
{
}

void ZenObserver::Notify(anemoia::Events event)
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

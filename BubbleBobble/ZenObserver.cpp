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
	}
}

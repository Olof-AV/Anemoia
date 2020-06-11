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
	}
}

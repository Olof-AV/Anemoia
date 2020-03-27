#include "AnemoiaPCH.h"
#include "Subject.h"
#include "Observer.h"

anemoia::Subject::~Subject()
{
	//Delete the observers
	std::for_each(m_pObservers.cbegin(), m_pObservers.cend(), [](Observer* const pObserver)
	{
		delete pObserver;
	});
}

void anemoia::Subject::AddObserver(Observer* const pObserver)
{
	m_pObservers.push_front(pObserver);
}

void anemoia::Subject::RemoveObserver(Observer* const pObserver)
{
	m_pObservers.remove(pObserver);
}

void anemoia::Subject::Notify(Events event)
{
	//All observers should be notified
	std::for_each(m_pObservers.cbegin(), m_pObservers.cend(), [event](Observer* const pObserver)
	{
		pObserver->Notify(event);
	});
}

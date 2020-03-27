#pragma once
#include "Event.h"
#include <list>

namespace anemoia
{
	class Observer;

	class Subject
	{
	public:
#pragma region Constructors
		Subject() = default;
		~Subject();

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;
#pragma endregion Constructors

		void AddObserver(Observer* const pObserver);
		void RemoveObserver(Observer* const pObserver);

	protected:
		void Notify(Events event);

	private:
		std::list<Observer*> m_pObservers;
	};
}

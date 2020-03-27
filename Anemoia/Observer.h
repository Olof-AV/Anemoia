#pragma once
#include "Event.h"

namespace anemoia
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Events event) = 0;
	};
}
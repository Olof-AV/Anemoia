#pragma once
#include "Observer.h"

class ZenBehaviour;

class ZenObserver final : public anemoia::Observer
{
public:
	explicit ZenObserver(ZenBehaviour* const pBehaviour);
	virtual ~ZenObserver() = default;
	virtual void Notify(anemoia::Events event) override;

private:
	ZenBehaviour* const m_pBehaviour;
};


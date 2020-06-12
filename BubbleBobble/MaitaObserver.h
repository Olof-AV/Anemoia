#pragma once
#include "Observer.h"

class MaitaBehaviour;

class MaitaObserver final : public anemoia::Observer
{
public:
	explicit MaitaObserver(MaitaBehaviour* const pBehaviour);
	virtual ~MaitaObserver() = default;
	virtual void Notify(anemoia::Events event) override;

private:
	MaitaBehaviour* const m_pBehaviour;
};


#include "AnemoiaPCH.h"
#include "FPSComponent.h"

anemoia::FPSComponent::FPSComponent(GameObject* const pParent)
	: BaseComponent{ pParent, Transform() }
{
	m_FPS = 60.f;
}

void anemoia::FPSComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void anemoia::FPSComponent::Update(float elapsedSec)
{
	m_FPS = 1.f / elapsedSec;
}

float anemoia::FPSComponent::GetFPS() const
{
	return m_FPS;
}

#include "AnemoiaPCH.h"
#include "FPSComponent.h"

FPSComponent::FPSComponent(GameObject* const pParent)
	: BaseComponent{ pParent, Transform() }
{
	m_FPS = 60.f;
}

void FPSComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void FPSComponent::Update(float elapsedSec)
{
	m_FPS = 1.f / elapsedSec;
}

float FPSComponent::GetFPS() const
{
	return m_FPS;
}

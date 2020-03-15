#include "AnemoiaPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

anemoia::BaseComponent::BaseComponent(GameObject* const pParent,
	const Transform& transform)
	: m_pParent{ pParent }, m_Transform{ transform }
{
}

#pragma region GettersSetters
const anemoia::Transform& anemoia::BaseComponent::GetTransform() const
{
	return m_Transform;
}

void anemoia::BaseComponent::SetTransform(const Transform& newTransform)
{
	m_Transform = newTransform;
}

anemoia::GameObject* const anemoia::BaseComponent::GetParent() const
{
	return m_pParent;
}

#pragma endregion GettersSetters

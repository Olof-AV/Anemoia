#include "AnemoiaPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

BaseComponent::BaseComponent(GameObject* const pParent,
	const Transform& transform)
	: m_pParent{ pParent }, m_Transform{ transform }
{
}

#pragma region GettersSetters
const Transform& BaseComponent::GetTransform() const
{
	return m_Transform;
}

void BaseComponent::SetTransform(const Transform& newTransform)
{
	m_Transform = newTransform;
}

GameObject* const BaseComponent::GetParent() const
{
	return m_pParent;
}

#pragma endregion GettersSetters

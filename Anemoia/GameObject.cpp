#include "AnemoiaPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "BaseComponent.h"
#include <algorithm>

anemoia::GameObject::GameObject(Scene* const pParentScene)
	: m_pParentScene{ pParentScene }
{
}

anemoia::GameObject::~GameObject()
{
	std::for_each(m_Components.cbegin(), m_Components.cend(), [](BaseComponent* const pComp)
	{
		delete pComp;
	});
}

void anemoia::GameObject::FixedUpdate(float timeStep)
{
	//Just fixed update (physics and stuff)
	std::for_each(m_Components.cbegin(), m_Components.cend(), [timeStep](BaseComponent* const pComp)
	{
		pComp->FixedUpdate(timeStep);
	});
}

void anemoia::GameObject::Update(float elapsedSec)
{
	//Just update
	std::for_each(m_Components.cbegin(), m_Components.cend(), [elapsedSec](BaseComponent* const pComp)
	{
		pComp->Update(elapsedSec);
	});
}

void anemoia::GameObject::Render() const
{
	//Just render
	std::for_each(m_Components.cbegin(), m_Components.cend(), [](BaseComponent* const pComp)
	{
		pComp->Render();
	});
}

#pragma region GettersSetters
const glm::vec3& anemoia::GameObject::GetPosition() const
{
	return m_Position;
}

void anemoia::GameObject::SetPosition(const glm::vec3& newPos)
{
	m_Position = newPos;
}

void anemoia::GameObject::SetPosition(float x, float y, float z)
{
	m_Position = glm::vec3(x, y, z);
}

anemoia::Scene* const anemoia::GameObject::GetParentScene() const
{
	return m_pParentScene;
}

void anemoia::GameObject::SetParentScene(Scene* const pScene)
{
	m_pParentScene = pScene;
}
#pragma endregion GettersSetters

void anemoia::GameObject::AddComponent(BaseComponent* const pComp)
{
	//Try to find component being added
	std::vector<BaseComponent*>::const_iterator cIt = std::find(m_Components.cbegin(), m_Components.cend(), pComp);

	//Only add if it's not already in there
	if (cIt == m_Components.cend())
	{
		m_Components.push_back(pComp);
	}
	else
	{
		throw std::runtime_error("Failed to add component: already present in this GameObject!");
	}
}

bool anemoia::GameObject::RemoveComponent(BaseComponent* const pComp)
{
	//Try to find component being removed
	std::vector<BaseComponent*>::const_iterator cIt = std::find(m_Components.cbegin(), m_Components.cend(), pComp);

	if (cIt != m_Components.cend())
	{
		//Found, so delete and erase from vector
		delete* cIt;
		m_Components.erase(cIt);
		
		//Was found
		return true;
	}

	//Was not found
	return false;
}

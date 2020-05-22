#include "AnemoiaPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

//unsigned int anemoia::Scene::m_IdCounter = 0;

anemoia::Scene::Scene(const std::string& name)
	: m_Name(name)
{

}

anemoia::Scene::~Scene()
{
	//Added objects are taken care of
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](GameObject* const pObject)
	{
		delete pObject;
	});
}

void anemoia::Scene::AddChild(GameObject* const pObject)
{
	//Just adds an object to the scene
	m_Objects.push_back(pObject);
	pObject->SetParentScene(this);
}

void anemoia::Scene::RemoveChild(GameObject* const pObject)
{
	//Check if in current scene
	const std::vector<GameObject*>::const_iterator cIt = std::find(m_Objects.cbegin(), m_Objects.cend(), pObject);

	//Exists?
	if (cIt != m_Objects.cend())
	{
		(*cIt)->SetMarkForDelete(true);
		m_ObjectsToDelete = true;
	}
}

void anemoia::Scene::FixedUpdate(float timeStep)
{
	//Remove objects we planned to delete
	if (m_ObjectsToDelete)
	{
		const std::vector<GameObject*>::const_iterator cIt = std::remove_if(m_Objects.begin(), m_Objects.end(), [](GameObject* const pObject)
		{
			if (pObject->GetMarkForDelete())
			{
				delete pObject;
				return true;
			}
			return false;
		});
		m_Objects.erase(cIt, m_Objects.cend());

		//Stop
		m_ObjectsToDelete = false;
	}

	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [timeStep](GameObject* const pObject)
	{
		if (pObject->IsEnabled())
		{
			pObject->FixedUpdate(timeStep);
		}
	});
}

void anemoia::Scene::Update(float elapsedSec)
{
	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [elapsedSec](GameObject* const pObject)
	{
		if (pObject->IsEnabled())
		{
			pObject->Update(elapsedSec);
		}
	});
}

void anemoia::Scene::LateUpdate(float elapsedSec)
{
	//Late Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [elapsedSec](GameObject* const pObject)
	{
		if (pObject->IsEnabled())
		{
			pObject->LateUpdate(elapsedSec);
		}
	});
}

void anemoia::Scene::Render() const
{
	//Render all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](GameObject* const pObject)
	{
		if (pObject->IsEnabled())
		{
			pObject->Render();
		}
	});
}

void anemoia::Scene::Initialise()
{
	m_IsInitialised = true;
}

void anemoia::Scene::OnSceneActivated()
{
}

void anemoia::Scene::OnSceneDeactivated()
{
}

bool anemoia::Scene::IsInitialised() const
{
	return m_IsInitialised;
}

const std::string& anemoia::Scene::GetName() const
{
	return m_Name;
}

void anemoia::Scene::SetName(const std::string& name)
{
	m_Name = name;
}

void anemoia::Scene::AddCollider(ColliderComponent* const pColl)
{
	//Add given collider to scene collision, will exist until removed
	m_Collision.emplace_back(pColl);
}

void anemoia::Scene::RemoveCollider(ColliderComponent* const pColl)
{
	//Find
	const std::vector<ColliderComponent*>::const_iterator cIt = std::find(m_Collision.cbegin(), m_Collision.cend(), pColl);

	//If found, erase
	if (cIt != m_Collision.cend())
	{
		m_Collision.erase(cIt);
	}
}

const std::vector<anemoia::ColliderComponent*>& anemoia::Scene::GetColliders() const
{
	return m_Collision;
}

anemoia::GameObject* anemoia::Scene::GetObjectWithTag(const std::string& tag) const
{
	//Find FIRST object with tag
	const std::vector<anemoia::GameObject*>::const_iterator cIt = std::find_if(m_Objects.cbegin(), m_Objects.cend(), [&tag](anemoia::GameObject* const pObject)
	{
		return pObject->HasTag(tag);
	});

	//Return, can be nullptr though
	if (cIt != m_Objects.cend())
	{
		return *cIt;
	}
	return nullptr;
}

std::vector<anemoia::GameObject*> anemoia::Scene::GetObjectsWithTag(const std::string& tag) const
{
	//Create vector
	std::vector<GameObject*> toReturn{};

	//Copy relevant objects
	std::copy_if(m_Objects.cbegin(), m_Objects.cend(), std::back_inserter(toReturn), [&tag](anemoia::GameObject* const pObject)
	{
		return pObject->HasTag(tag);
	});

	//Return result
	return toReturn;
}


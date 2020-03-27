#include "AnemoiaPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

//unsigned int anemoia::Scene::m_IdCounter = 0;

anemoia::Scene::Scene(const std::string& name)
	: m_Name(name)
{
	Initialise();
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

void anemoia::Scene::FixedUpdate(float timeStep)
{
	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [timeStep](GameObject* const pObject)
	{
		pObject->FixedUpdate(timeStep);
	});
}

void anemoia::Scene::Update(float elapsedSec)
{
	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [elapsedSec](GameObject* const pObject)
	{
		pObject->Update(elapsedSec);
	});
}

void anemoia::Scene::LateUpdate(float elapsedSec)
{
	//Late Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [elapsedSec](GameObject* const pObject)
	{
		pObject->LateUpdate(elapsedSec);
	});
}

void anemoia::Scene::Render() const
{
	//Render all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](GameObject* const pObject)
	{
		pObject->Render();
	});
}

void anemoia::Scene::Initialise()
{
}

const std::string& anemoia::Scene::GetName() const
{
	return m_Name;
}


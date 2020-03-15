#include "AnemoiaPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
{
	Initialise();
}

Scene::~Scene()
{
	//Added objects are taken care of
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](GameObject* const pObject)
	{
		delete pObject;
	});
}

void Scene::AddChild(GameObject* const pObject)
{
	//Just adds an object to the scene
	m_Objects.push_back(pObject);
}

void Scene::FixedUpdate(float timeStep)
{
	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [timeStep](GameObject* const pObject)
	{
		pObject->FixedUpdate(timeStep);
	});
}

void Scene::Update(float elapsedSec)
{
	//Update all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [elapsedSec](GameObject* const pObject)
	{
		pObject->Update(elapsedSec);
	});
}

void Scene::Render() const
{
	//Render all objects
	std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](GameObject* const pObject)
	{
		pObject->Render();
	});
}

void Scene::Initialise()
{
}


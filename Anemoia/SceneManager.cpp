#include "AnemoiaPCH.h"
#include "SceneManager.h"
#include "Scene.h"

#include <algorithm>

void anemoia::SceneManager::FixedUpdate(float timeStep)
{
	//Just update all for now (please fix)
	std::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [timeStep](Scene* const pScene)
	{
		pScene->FixedUpdate(timeStep);
	});
}

void anemoia::SceneManager::Update(float elapsedSec)
{
	//Just update all for now (please fix)
	std::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [elapsedSec](Scene* const pScene)
	{
		pScene->Update(elapsedSec);
	});
}

void anemoia::SceneManager::Render()
{
	//Just render all for now (please fix)
	std::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [](Scene* const pScene)
	{
		pScene->Render();
	});
}

void anemoia::SceneManager::AddScene(Scene* const pScene)
{
	//Try to find scene being added
	std::vector<Scene*>::const_iterator cIt = std::find(m_Scenes.cbegin(), m_Scenes.cend(), pScene);

	//Only add if it's not already in there
	if (cIt == m_Scenes.cend())
	{
		m_Scenes.push_back(pScene);
	}
	else
	{
		throw std::runtime_error("Failed to add scene: already present in SceneManager!");
	}
}

void anemoia::SceneManager::InitialiseScenes() const
{
	//Just initialise all scenes
	std::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [](Scene* const pScene)
	{
		pScene->Initialise();
	});
}

anemoia::SceneManager::~SceneManager()
{
	//Delete managed scenes
	std::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [](Scene* const pScene)
	{
		delete pScene;
	});
}

/*
Scene* SceneManager::CreateScene(const std::string& name)
{
	Scene* const pScene = new Scene(name);
	m_Scenes.push_back(pScene);
	return pScene;
}
*/
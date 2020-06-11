#include "AnemoiaPCH.h"
#include "SceneManager.h"
#include "Scene.h"

#include <algorithm>

#include "AIManager.h"

void anemoia::SceneManager::FixedUpdate(float timeStep)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->FixedUpdate(timeStep);
	};
}

void anemoia::SceneManager::Update(float elapsedSec)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update(elapsedSec);
	}
}

void anemoia::SceneManager::LateUpdate(float elapsedSec)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->LateUpdate(elapsedSec);
	}
}

void anemoia::SceneManager::Render()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Render();
	}
}

void anemoia::SceneManager::AddScene(Scene* const pScene)
{
	//Try to find scene being added
	std::vector<Scene*>::const_iterator cIt = std::find(m_Scenes.cbegin(), m_Scenes.cend(), pScene);

	//Only add if it's not already in there
	if (cIt == m_Scenes.cend())
	{
		//Pause -- new AI might be added
		anemoia::AIManager::GetInstance()->Pause(true);

		//Add to known scenes
		m_Scenes.emplace_back(pScene);

		//Initialise?
		if (!pScene->IsInitialised())
		{
			pScene->Initialise();
		}

		//When finished, go on
		anemoia::AIManager::GetInstance()->Pause(false);
	}
	else
	{
		throw std::runtime_error("Failed to add scene: already present in SceneManager!");
	}
}

anemoia::Scene* const anemoia::SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}

void anemoia::SceneManager::SetActiveScene(Scene* const pScene)
{
	//Try to find scene being asked to put active
	std::vector<Scene*>::const_iterator cIt = std::find(m_Scenes.cbegin(), m_Scenes.cend(), pScene);

	//Only set as active if found
	if (cIt != m_Scenes.cend())
	{
		//Call back
		if (m_pActiveScene) { m_pActiveScene->OnSceneDeactivated(); }

		//Change active scene
		m_pActiveScene = *cIt;

		//Callback
		if (m_pActiveScene) { m_pActiveScene->OnSceneActivated(); }
	}
	else
	{
		throw std::runtime_error("Failed to set scene as active, not present in manager!");
	}
}

void anemoia::SceneManager::SetActiveScene(const std::string& name)
{
	//Try to find scene being asked to put active
	std::vector<Scene*>::const_iterator cIt = std::find_if(m_Scenes.cbegin(), m_Scenes.cend(), [&name](Scene* const pScene)
	{
		return pScene->GetName() == name;
	});

	//Only set as active if found
	if (cIt != m_Scenes.cend())
	{
		//Call back
		if (m_pActiveScene) { m_pActiveScene->OnSceneDeactivated(); }

		//Change active scene
		m_pActiveScene = *cIt;

		//Callback
		if (m_pActiveScene) { m_pActiveScene->OnSceneActivated(); }
	}
	else
	{
		throw std::runtime_error("Failed to set scene as active, not present in manager!");
	}
}

bool anemoia::SceneManager::IsSceneActive(Scene* const pScene)
{
	if (m_pActiveScene)
	{
		return pScene == m_pActiveScene;
	}
	return false;
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
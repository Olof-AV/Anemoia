#include "AnemoiaPCH.h"
#include "AIManager.h"

#include "SceneManager.h"

anemoia::AIManager::AIManager()
{
	m_Functions.clear();
}

anemoia::AIManager::~AIManager()
{
	m_Functions.clear();
}

void anemoia::AIManager::RegisterFunction(const BoundFunc& func)
{
	//Is it already present in the vector?
	const std::vector<BoundFunc>::const_iterator cIt = std::find(m_Functions.cbegin(), m_Functions.cend(), func);

	//If not, add
	if (cIt == m_Functions.cend())
	{
		m_Functions.emplace_back(func);
	}
	//Otherwise, error
	else
	{
		throw std::runtime_error("Failed to add function: already present in AIManager!");
	}
}

void anemoia::AIManager::UnRegisterFunction(const BoundFunc &func)
{
	//Is it already present in the vector?
	const std::vector<BoundFunc>::const_iterator cIt = std::remove(m_Functions.begin(), m_Functions.end(), func);

	//If found, remove
	if (cIt != m_Functions.cend())
	{
		m_Functions.erase(cIt);
	}
}

void anemoia::AIManager::Run()
{
	m_IsRunning = true;

	//Runs in a thread until told to stop
	while(m_IsRunning)
	{
		//Get scene manager, only active scenes will have their AI running
		SceneManager* const pMan = SceneManager::GetInstance();

		//Go through registered AI functions
		for (size_t i{}; i < m_Functions.size(); ++i)
		{
			if (!m_IsRunning) { return; } //Quit?

			if (pMan->IsSceneActive(m_Functions[i].pScene) && m_Functions[i].func)
			{
				m_Functions[i].func();
			}
		}
	}
}

void anemoia::AIManager::Stop()
{
	m_IsRunning = false;
}

bool anemoia::operator==(const anemoia::BoundFunc& a, const anemoia::BoundFunc& b)
{
	return a.pParent == b.pParent && a.pScene == b.pScene;
}

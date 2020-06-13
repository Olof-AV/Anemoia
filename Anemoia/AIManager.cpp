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
	//Lock -- thread might be running
	const std::lock_guard<std::mutex> lock(m_Mutex);

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
		throw std::runtime_error("Failed to add function: parent object already has a registered function in AIManager!");
	}
}

void anemoia::AIManager::UnRegisterFunction(const BoundFunc &func)
{
	//Lock -- thread might be running
	const std::lock_guard<std::mutex> lock(m_Mutex);

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
		if (!m_IsPaused)
		{
			//Get scene manager, only active scenes will have their AI running
			SceneManager* const pMan = SceneManager::GetInstance();

			//Go through registered AI functions
			const size_t max = m_Functions.size();
			for (size_t i{}; i < max; ++i)
			{
				if (!m_IsRunning) { return; } //Quit?
				if (m_IsPaused) { break; } //Pause?

				if (pMan->IsSceneActive(m_Functions[i].pScene) && m_Functions[i].func)
				{
					m_Functions[i].func();
				}
			}
		}
	}
}

void anemoia::AIManager::Stop()
{
	m_IsRunning = false;
}

void anemoia::AIManager::Pause(bool isPaused)
{
	m_IsPaused = isPaused;
}

bool anemoia::operator==(const anemoia::BoundFunc& a, const anemoia::BoundFunc& b)
{
	return a.pParent == b.pParent && a.pScene == b.pScene;
}

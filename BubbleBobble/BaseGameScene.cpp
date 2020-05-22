#include "pch.h"
#include "BaseGameScene.h"

#include "Locator.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include "TextureComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

#include "HUDComponent.h"
#include "PlayerBehaviour.h"
#include "ZenBehaviour.h"
#include "MaitaBehaviour.h"

#include "PlayerObserver.h"

#include <regex>
#include <fstream>

#include "SceneManager.h"

#include "BubbleBobbleGame.h"

BaseGameScene::BaseGameScene(UINT levelNum)
	: Scene("Stage" + std::to_string(levelNum)), m_LevelNum(levelNum)
{
	m_Enemies.clear();

	//Params
	m_EndTimerActive = false;
	m_EndTimer = 0.f;
	m_EndTimerMax = 5.f;
}

void BaseGameScene::FixedUpdate(float timeStep)
{
	//Call root
	Scene::FixedUpdate(timeStep);
}

void BaseGameScene::Update(float elapsedSec)
{
	//Call root
	Scene::Update(elapsedSec);

	//If the end timer is active, it will eventually switch to the next stage
	if (m_EndTimerActive)
	{
		m_EndTimer += elapsedSec;
		if (m_EndTimer > m_EndTimerMax)
		{
			anemoia::SceneManager::GetInstance()->SetActiveScene("Stage" + std::to_string(m_LevelNum + 1));
		}
	}
}

void BaseGameScene::LateUpdate(float elapsedSec)
{
	//Call root
	Scene::LateUpdate(elapsedSec);
}

void BaseGameScene::Render() const
{
	//Call root
	Scene::Render();
}

void BaseGameScene::Initialise()
{
	//Call root
	Scene::Initialise();

	//Environment
	ReadLevelData();

	//Add HUD
	{
		//Root
		m_pHUD = new anemoia::GameObject(this);

		//Initialise hud component + add it
		anemoia::Font* const pFont = anemoia::ResourceManager::GetInstance()->LoadFont("Fonts/Pixel_NES.otf", 21);
		anemoia::Texture2D* const pTexP1 = anemoia::ResourceManager::GetInstance()->LoadTexture("HUD/Health_P1.png");
		anemoia::Texture2D* const pTexP2 = anemoia::ResourceManager::GetInstance()->LoadTexture("HUD/Health_P2.png");
		HUDComponent* const pHudComp = new HUDComponent(m_pHUD, anemoia::Transform{}, pFont, pTexP1, pTexP2);
		m_pHUD->AddComponent(pHudComp);

		//Tag
		m_pHUD->AddTag("HUD");

		//Add root to scene
		AddChild(m_pHUD);
	}

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
}

void BaseGameScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();

	//Update HUD
	m_pHUD->GetComponent<HUDComponent>()->UpdateScores();
}

void BaseGameScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
}

void BaseGameScene::NotifyEnemyDeath(anemoia::GameObject* const pObj)
{
	//If enemies left
	if (m_Enemies.size() > 0)
	{
		//Try to remove the given enemy
		const std::vector<anemoia::GameObject*>::const_iterator cIt = std::find(m_Enemies.cbegin(), m_Enemies.cend(), pObj);

		//If found, erase it
		if (cIt != m_Enemies.cend())
		{
			m_Enemies.erase(cIt);

			//If no enemies left, start the end timer (which will switch to next stage)
			if (m_Enemies.size() == 0)
			{
				m_EndTimerActive = true;
			}
		}
	}
}

void BaseGameScene::ReadLevelData()
{
	//Open file
	const std::string path = "../Data/Levels/" + std::to_string(m_LevelNum) + "/Data.txt";
	std::ifstream fileStream{ path };

	//Read until file end
	while (fileStream.good())
	{
		//Read one line
		std::string input;
		std::getline(fileStream, input);

		//Safeguard from empty line
		if (input.size() > 0)
		{
			if (CheckDataForBigTile(input))
			{
				continue;
			}
			if (CheckDataForBigTileInvis(input))
			{
				continue;
			}
			if (CheckDataForTile(input))
			{
				continue;
			}
			if (CheckDataForFakeTile(input))
			{
				continue;
			}
			if (CheckDataForTileHard(input))
			{
				continue;
			}
			if (CheckDataForZenChan(input))
			{
				continue;
			}
			if (CheckDataForMaita(input))
			{
				continue;
			}
			if (CheckDataForPlayer(input))
			{
				continue;
			}
		}
	}
}

bool BaseGameScene::CheckDataForFakeTile(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:FakeTile=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateTile(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), false, false, false, false);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForTile(const std::string &input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:Tile=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateTile(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), false, false, false);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForTileHard(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:TileHard=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateTile(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), false, true, false);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForBigTile(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:BigTile=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateTile(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), true, true, false, true);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForBigTileInvis(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:BigTileInvis=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateTile(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), true, true, true);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForZenChan(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:ZenChan=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			//Create ZenChan
			{
				//Root
				anemoia::GameObject* const pZen = new anemoia::GameObject(this);
				pZen->SetPosition(std::stof(matches[1]), std::stof(matches[2]), 0.f);

				//Texture
				anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
				anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pZen, transform, nullptr);
				pZen->AddComponent(pTexComp);

				//Collision
				anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pZen, transform, glm::vec2(48.f, 48.f));
				pZen->AddComponent(pColl);

				//Rigid body
				anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pZen, pColl);
				pRigid->AddIgnoreTag("ZenChan");
				pRigid->AddIgnoreTag("Maita");
				pRigid->AddIgnoreTag("Treasure");
				pZen->AddComponent(pRigid);

				//Behaviour
				ZenBehaviour* const pBehaviour = new ZenBehaviour(pZen, pRigid, pTexComp);
				pZen->AddComponent(pBehaviour);

				//Tag
				pZen->AddTag("ZenChan");

				//Add to scene
				AddChild(pZen);

				//This is an enemy
				m_Enemies.emplace_back(pZen);
			}

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForMaita(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:Maita=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 2 matches + 1 original match
		if (matches.size() == 3)
		{
			CreateMaita(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), false);

			return true;
		}
	}

	return false;
}

bool BaseGameScene::CheckDataForPlayer(const std::string& input)
{
	//Reads a block of tile data, with x/y coordinate
	const std::regex myRegex{ R"(^(?:Player=)([-]?\d+(?:\.\d+)?),\s*([-]?\d+(?:\.\d+)?),\s*(\w+)$)" };

	//Store matches in here
	std::smatch matches;

	//Search using the regex
	if (std::regex_search(input, matches, myRegex))
	{
		//Only can accept 3 matches + 1 original match
		if (matches.size() == 4)
		{
			//Add players and stuff
			{
				//Is this P1?
				const bool isP1 = (matches[3].str().compare("true") == 0) ? true : false;

				if (!isP1)
				{
					//Depending on gamemode, P2 will be different
					Gamemode mode = static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->GetGamemode();
					switch (mode)
					{
					case Gamemode::singleplayer:
						break; //NO P2

					case Gamemode::multiplayer:
						CreatePlayer(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), isP1);
						break;

					case Gamemode::versus:
						
						//No behaviour yet
						break;
					}
				}
				else
				{
					CreatePlayer(glm::vec2(std::stof(matches[1]), std::stof(matches[2])), isP1);
				}
			}

			return true;
		}
	}

	return false;
}

void BaseGameScene::CreateMaita(const glm::vec2& pos, bool isPlayer)
{
	//Root
	anemoia::GameObject* const pMaita = new anemoia::GameObject(this);
	pMaita->SetPosition(pos.x, pos.y, 0.f);

	//Texture
	anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
	anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pMaita, transform, nullptr);
	pMaita->AddComponent(pTexComp);

	//Collision
	anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pMaita, transform, glm::vec2(48.f, 48.f));
	pMaita->AddComponent(pColl);

	//Rigid body
	anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pMaita, pColl);
	pRigid->AddIgnoreTag("ZenChan");
	pRigid->AddIgnoreTag("Maita");
	pRigid->AddIgnoreTag("Treasure");
	pMaita->AddComponent(pRigid);

	//Behaviour
	MaitaBehaviour* const pBehaviour = new MaitaBehaviour(pMaita, pRigid, pTexComp, isPlayer);
	pMaita->AddComponent(pBehaviour);

	//Tag
	pMaita->AddTag("Maita");

	//Add to scene
	AddChild(pMaita);

	//This is an enemy
	m_Enemies.emplace_back(pMaita);
}

void BaseGameScene::CreatePlayer(const glm::vec2& pos, bool isP1)
{
	//Root
	anemoia::GameObject* const pPlayer = new anemoia::GameObject(this);
	pPlayer->SetPosition(pos.x, pos.y, 0.f);

	//Texture
	anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
	anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pPlayer, transform, nullptr);
	pPlayer->AddComponent(pTexComp);

	//Collision
	anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pPlayer, transform, glm::vec2(48.f, 48.f));
	pPlayer->AddComponent(pColl);

	//Rigid body
	anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pPlayer, pColl);
	pRigid->AddIgnoreTag("Player");
	pPlayer->AddComponent(pRigid);

	//Behaviour
	PlayerBehaviour* const pBehaviour = new PlayerBehaviour(pPlayer, pRigid, pTexComp, !isP1);
	pPlayer->AddComponent(pBehaviour);

	//Tag
	pPlayer->AddTag("Player");

	//Observer
	pPlayer->AddObserver(new PlayerObserver(isP1));

	//Add to scene
	AddChild(pPlayer);
}

void BaseGameScene::CreateTile(const glm::vec2& pos, bool isBig, bool isImportant, bool isInvisible, bool hasCollision)
{
	//Root
	anemoia::GameObject* const pWall = new anemoia::GameObject(this);
	pWall->SetPosition(pos.x, pos.y , 0.f);

	//Sprite
	if (!isInvisible)
	{
		anemoia::Texture2D* const pTex2D = anemoia::ResourceManager::GetInstance()->LoadTexture("Levels/" + std::to_string(m_LevelNum) + ((isBig) ? "/Big.png" : "/Small.png"));
		anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pWall, anemoia::Transform(glm::vec3(), glm::vec2(0.f, 1.f)), pTex2D);
		pWall->AddComponent(pTexComp);
	}

	//Collision
	if (hasCollision)
	{
		const float size = (isBig) ? 48.f : 24.f;
		anemoia::ColliderComponent* const pCollider =
			new anemoia::ColliderComponent(pWall, anemoia::Transform(glm::vec3(), glm::vec2(0.f, 1.f)), glm::vec2(size, size), isImportant);
		pWall->AddComponent(pCollider);
	}

	//Add to scene
	AddChild(pWall);
}

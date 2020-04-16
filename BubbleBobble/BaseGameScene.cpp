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

#include <regex>
#include <fstream>

BaseGameScene::BaseGameScene(UINT levelNum)
	: Scene("Stage" + std::to_string(levelNum)), m_LevelNum(levelNum)
{
}

BaseGameScene::~BaseGameScene()
{
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

		//Add root to scene
		AddChild(m_pHUD);
	}

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Add players and stuff
	{
		//Root
		anemoia::GameObject* const pBubby = new anemoia::GameObject(this);

		//Texture
		anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
		anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pBubby, transform, nullptr);
		pBubby->AddComponent(pTexComp);

		//Collision
		anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pBubby, transform, glm::vec2(48.f, 48.f));
		pBubby->AddComponent(pColl);

		//Rigid body
		anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pBubby, pColl);
		pBubby->AddComponent(pRigid);

		//Behaviour
		PlayerBehaviour* const pBehaviour = new PlayerBehaviour(pBubby, pRigid, pTexComp);
		pBubby->AddComponent(pBehaviour);

		//Tag
		pBubby->AddTag("Player");

		//Add to scene
		pBubby->SetPosition(glm::vec3(x * 0.12f, y * 0.95f, 0.f));
		AddChild(pBubby);
	}
}

void BaseGameScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();
}

void BaseGameScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
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
				anemoia::GameObject* const pBubby = new anemoia::GameObject(this);
				pBubby->SetPosition(std::stof(matches[1]), std::stof(matches[2]), 0.f);

				//Texture
				anemoia::Texture2D* pTex = anemoia::ResourceManager::GetInstance()->LoadTexture("Enemies/ZenChan/Run.png");
				anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
				anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pBubby, transform, pTex);
				pBubby->AddComponent(pTexComp);

				//Collision
				anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pBubby, transform, glm::vec2(48.f, 48.f));
				pBubby->AddComponent(pColl);

				//Rigid body
				anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pBubby, pColl);
				pBubby->AddComponent(pRigid);

				//Tag
				pBubby->AddTag("ZenChan");

				//Add to scene
				AddChild(pBubby);
			}

			return true;
		}
	}

	return false;
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

#include "pch.h"
#include "BaseGameScene.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "ResourceManager.h"

#include "Locator.h"
#include "HUDComponent.h"
#include "PlayerControllerComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

BaseGameScene::BaseGameScene(const std::string& name)
	: Scene(name)
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

	//Environment
	AddVerticalWalls();

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Add players and stuff
	{
		//Root
		anemoia::GameObject* const pBubby = new anemoia::GameObject(this);

		//Texture
		anemoia::Texture2D* const pTex = anemoia::ResourceManager::GetInstance()->LoadTexture("Player/Bubby.png");
		anemoia::Transform transform = anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 1.f));
		anemoia::TextureComponent* const pTexComp = new anemoia::TextureComponent(pBubby, transform, pTex);
		pBubby->AddComponent(pTexComp);

		//Collision
		anemoia::ColliderComponent* const pColl = new anemoia::ColliderComponent(pBubby, transform, glm::vec2(48.f, 48.f));
		pBubby->AddComponent(pColl);

		//Rigid body
		anemoia::RigidBodyComponent* const pRigid = new anemoia::RigidBodyComponent(pBubby, pColl);
		pBubby->AddComponent(pRigid);

		//Control
		anemoia::PlayerControllerComponent* const pControl = new anemoia::PlayerControllerComponent(pBubby);
		pBubby->AddComponent(pControl);

		//Add to scene
		//pBubby->SetPosition(glm::vec3(x * 0.5f, y * 0.75f, 0.f));
		pBubby->SetPosition(glm::vec3(x * 0.5f, 0.f, 0.f));
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

void BaseGameScene::AddVerticalWalls()
{
	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	for (int i{}; i < 13; ++i)
	{
		//Left
		{
			//Root
			anemoia::GameObject* const pWall = new anemoia::GameObject(this);
			pWall->SetPosition(0.f, y - 48.f * i, 0.f);

			//Collision
			anemoia::ColliderComponent* const pCollider =
				new anemoia::ColliderComponent(pWall, anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec2(1.f, 1.f)), glm::vec2(48.f, 48.f));
			pWall->AddComponent(pCollider);

			//Add to scene
			AddChild(pWall);
		}

		//Right
		{
			//Root
			anemoia::GameObject* const pWall = new anemoia::GameObject(this);
			pWall->SetPosition(x - 48.f, y - 48.f * i, 0.f);

			//Collision
			anemoia::ColliderComponent* const pCollider =
				new anemoia::ColliderComponent(pWall, anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 1.f)), glm::vec2(48.f, 48.f));
			pWall->AddComponent(pCollider);

			//Add to scene
			AddChild(pWall);
		}
	}

	for (int i{}; i < 28; ++i)
	{
		//Bottom
		{
			//Root
			anemoia::GameObject* const pWall = new anemoia::GameObject(this);
			pWall->SetPosition(48.f + 24.f * i, (float)y, 0.f);

			//Collision
			anemoia::ColliderComponent* const pCollider =
				new anemoia::ColliderComponent(pWall, anemoia::Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 1.f)), glm::vec2(24.f, 24.f));
			pWall->AddComponent(pCollider);

			//Add to scene
			AddChild(pWall);
		}
	}
}

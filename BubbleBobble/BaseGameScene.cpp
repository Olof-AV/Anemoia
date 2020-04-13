#include "pch.h"
#include "BaseGameScene.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "ResourceManager.h"

#include "Locator.h"
#include "HUDComponent.h"
#include "PlayerControllerComponent.h"

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

		//Control
		anemoia::PlayerControllerComponent* const pControl = new anemoia::PlayerControllerComponent(pBubby, anemoia::Transform());
		pBubby->AddComponent(pControl);

		//Add to scene
		pBubby->SetPosition(glm::vec3(x * 0.5f, y, 0.f));
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

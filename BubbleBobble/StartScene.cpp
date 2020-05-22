#include "pch.h"
#include "StartScene.h"

#include "Font.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Locator.h"
#include "BubbleBobbleGame.h"

#include "InputManager.h"
#include "Command.h"

#include "IntroScene.h"
#include "BaseGameScene.h"
#include "SceneManager.h"

StartScene::StartScene()
	: Scene("StartScene")
{
	//Inputs for starting the game
	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("sp", this, 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::singleplayer);
		m_Start = true;
	}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("mp", this, 0, XINPUT_GAMEPAD_DPAD_UP, VK_UP, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::multiplayer);
		m_Start = true;
	}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("vs", this, 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::versus);
		m_Start = true;
	}));
}

StartScene::~StartScene()
{
}

void StartScene::FixedUpdate(float timeStep)
{
	//Call root
	Scene::FixedUpdate(timeStep);
}

void StartScene::Update(float elapsedSec)
{
	//Call root
	Scene::Update(elapsedSec);

	if (m_Start)
	{
		StartGame();
	}
}

void StartScene::LateUpdate(float elapsedSec)
{
	//Call root
	Scene::LateUpdate(elapsedSec);
}

void StartScene::Render() const
{
	//Call root
	Scene::Render();
}

void StartScene::Initialise()
{
	//Call root
	Scene::Initialise();

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Create text
	{
		//Get font
		anemoia::Font* const pFont = anemoia::ResourceManager::GetInstance()->LoadFont("Fonts/Pixel_NES.otf", 24);

		//Create text holder
		anemoia::GameObject* const pRoot = new anemoia::GameObject(this);
		anemoia::TextComponent* pText1 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -100.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"<- for singleplayer", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* pText2 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"^ for multiplayer", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* pText3 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 0.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"-> for versus", pFont, SDL_Color{ 255, 255, 255 });

		//Add them
		pRoot->AddComponent(pText1);
		pRoot->AddComponent(pText2);
		pRoot->AddComponent(pText3);

		//Move text
		pRoot->SetPosition(0.5f * x, 0.95f * y, 0.f);

		//Add to scene
		AddChild(pRoot);
	}
}

void StartScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();
}

void StartScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
}

void StartScene::StartGame()
{
	//Add scenes
	{
		//Game intro scene
		anemoia::Scene* const pIntro = new IntroScene();
		anemoia::SceneManager::GetInstance()->AddScene(pIntro);
		//anemoia::SceneManager::GetInstance()->SetActiveScene(pIntro);

		//Level 1 scene
		anemoia::Scene* const pLevel1 = new BaseGameScene(1);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel1);

		//Level 2 scene
		anemoia::Scene* const pLevel2 = new BaseGameScene(2);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel2);

		//Level 3 scene
		anemoia::Scene* const pLevel3 = new BaseGameScene(3);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel3);
	}

	//Go to intro
	anemoia::SceneManager::GetInstance()->SetActiveScene("IntroScene");
}

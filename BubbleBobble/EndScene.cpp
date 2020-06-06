#include "pch.h"
#include "EndScene.h"

#include "InputManager.h"
#include "Command.h"
#include "BubbleBobbleGame.h"

#include "Font.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"

EndScene::EndScene()
	: Scene("EndScene")
{
	//Inputs for starting the game
	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("sp", this, 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, anemoia::ButtonState::Down, [this]()
		{
			static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->Restart();
		}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("vs", this, 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, anemoia::ButtonState::Down, [this]()
		{
			static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->Exit();
		}));
}

EndScene::~EndScene()
{
}

void EndScene::FixedUpdate(float timeStep)
{
	//Call root
	Scene::FixedUpdate(timeStep);
}

void EndScene::Update(float elapsedSec)
{
	//Call root
	Scene::Update(elapsedSec);
}

void EndScene::LateUpdate(float elapsedSec)
{
	//Call root
	Scene::LateUpdate(elapsedSec);
}

void EndScene::Render() const
{
	//Call root
	Scene::Render();
}

void EndScene::Initialise()
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
		anemoia::TextComponent* const pText1 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -100.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"Thanks for playing!", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* const pText2 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"<- to restart", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* const pText3 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 0.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"-> to quit", pFont, SDL_Color{ 255, 255, 255 });

		//Add them
		pRoot->AddComponent(pText1);
		pRoot->AddComponent(pText2);
		pRoot->AddComponent(pText3);

		//Move whole
		pRoot->SetPosition(0.5f * x, 0.95f * y, 0.f);

		//Add to scene
		AddChild(pRoot);
	}
}

void EndScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();
}

void EndScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
}
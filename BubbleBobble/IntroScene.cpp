#include "pch.h"
#include "IntroScene.h"

#include "GameObject.h"
#include "Font.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Locator.h"

IntroScene::IntroScene()
	: Scene{ "IntroScene" }
{
}

IntroScene::~IntroScene()
{
}

void IntroScene::FixedUpdate(float timeStep)
{
	//Call root fixed update
	Scene::FixedUpdate(timeStep);
}

void IntroScene::Update(float elapsedSec)
{
	//Call root update
	Scene::Update(elapsedSec);
}

void IntroScene::LateUpdate(float elapsedSec)
{
	//Call root late update
	Scene::LateUpdate(elapsedSec);
}

void IntroScene::Render() const
{
	//Call root render
	Scene::Render();
}

void IntroScene::Initialise()
{
	//Call root initialise
	Scene::Initialise();

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Create text
	{
		//Get font
		anemoia::Font* const pFont = anemoia::ResourceManager::GetInstance()->LoadFont("Pixel_NES.otf", 11);

		//Create text holder
		anemoia::GameObject* const pRoot = new anemoia::GameObject(this);
		anemoia::TextComponent* pText1 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"NOW, IT IS BEGINNING OF A", pFont, SDL_Color{255, 0, 0});
		anemoia::TextComponent* pText2 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -25.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"FANTASTIC STORY!! LET'S MAKE A", pFont, SDL_Color{ 255, 0, 0 });
		anemoia::TextComponent* pText3 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 0.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"JOURNEY TO THE CAVE OF MONSTERS!", pFont, SDL_Color{ 255, 0, 0 });
		anemoia::TextComponent* pText4 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 25.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"GOOD LUCK!", pFont, SDL_Color{ 255, 0, 0 });

		//Add them
		pRoot->AddComponent(pText1);
		pRoot->AddComponent(pText2);
		pRoot->AddComponent(pText3);
		pRoot->AddComponent(pText4);

		//Move text
		pRoot->SetPosition(0.5f * x, 0.35f * y, 0.f);

		//Add to scene
		AddChild(pRoot);
	}
}

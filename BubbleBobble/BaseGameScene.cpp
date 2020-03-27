#include "pch.h"
#include "BaseGameScene.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Locator.h"

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

		pBubby->SetPosition(glm::vec3(x * 0.5f, y, 0.f));
		AddChild(pBubby);
	}
}

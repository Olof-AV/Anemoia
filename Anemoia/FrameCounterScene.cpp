#include "AnemoiaPCH.h"
#include "FrameCounterScene.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "FPSComponent.h"

#include <SDL.h>

#include "Locator.h"

anemoia::FrameCounterScene::FrameCounterScene()
	: Scene("FrameCounterScene")
{
}

anemoia::FrameCounterScene::~FrameCounterScene()
{
}

void anemoia::FrameCounterScene::FixedUpdate(float timeStep)
{
	//Call root fixed update
	Scene::FixedUpdate(timeStep);

	//Update text with elapsed Sec as text
	m_pText->SetText(std::to_string( (int)m_pFPSComp->GetFPS()));
}

void anemoia::FrameCounterScene::Update(float elapsedSec)
{
	//Call root update
	Scene::Update(elapsedSec);

	//Rotate
	Transform transform = m_pText->GetTransform();
	transform.SetAngle(transform.GetAngle() + elapsedSec * 50.f);
	m_pText->SetTransform(transform);
}

void anemoia::FrameCounterScene::Render() const
{
	//Call root render
	Scene::Render();
}

void anemoia::FrameCounterScene::Initialise()
{
	//Call root initialise
	Scene::Initialise();

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(Locator::GetWindow(), &x, &y);

	//Set up counter object
	GameObject* const pCounter = new GameObject();
	//pCounter->SetPosition(0.25f * x, 0.25f * y, 0.f); //Try this to show multiple transforms
	Font* const pFont = ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36);

	//Create backdrop
	{
		const Transform newTransform = Transform(glm::vec3(0.5f * x, 0.5f * y, 0.f), glm::vec2(0.5f, 0.5f), glm::vec2(0.9f, 0.9f));
		Texture2D* const pTex = ResourceManager::GetInstance()->LoadTexture("background.jpg");
		TextureComponent* const pTexComp = new TextureComponent(pCounter, newTransform, pTex);
		pCounter->AddComponent(pTexComp);
	}

	//Create text comp and add to object
	{
		const Transform newTransform = Transform(glm::vec3(0.5f * x, 0.5f * y, 0.f), glm::vec2(0.5f, 0.5f), glm::vec2(2.f, 2.f));
		m_pText = new TextComponent(pCounter, newTransform, "0", pFont, SDL_Colour{ 255, 255, 0 });
		pCounter->AddComponent(m_pText);
	}

	//Create FPS comp and add to object
	{
		m_pFPSComp = new FPSComponent(pCounter);
		pCounter->AddComponent(m_pFPSComp);
	}

	//Add to scene
	AddChild(pCounter);
}

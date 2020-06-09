#include "pch.h"
#include "IntroScene.h"

#include "GameObject.h"
#include "Font.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Locator.h"

#include "SceneManager.h"

#include "AnimSpriteComponent.h"

IntroScene::IntroScene()
	: Scene{ "IntroScene" }
{
	//Transition opacity
	m_TransitionAlpha = 1.f;
	m_TransitionAlphaTarget = 1.f;
	m_TransitionSpeed = 8.f;
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

	//Timer
	m_CurrentTime += elapsedSec;
	if (m_MaxTime < m_CurrentTime)
	{
		m_TransitionAlphaTarget = 1.f;
		if (m_TransitionAlpha > 0.99f)
		{
			anemoia::SceneManager::GetInstance()->SetActiveScene("Stage1");
		}
	}

	//Move characters
	const float mult = 5.f;
	m_pP1->SetPosition(m_pP1->GetPosition() + glm::vec3(std::cosf(m_CurrentTime * mult), std::sinf(m_CurrentTime * mult), 0.f));
	m_pP2->SetPosition(m_pP2->GetPosition() - glm::vec3(std::cosf(m_CurrentTime * mult), std::sinf(m_CurrentTime * mult), 0.f));

	//Update transition opacity by lerping to target
	m_TransitionAlpha = m_TransitionAlpha + (m_TransitionAlphaTarget - m_TransitionAlpha) * elapsedSec * m_TransitionSpeed;
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

	//Draw transition block, window size needed
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
	{
		SDL_Rect winRect;
		winRect.x = 0; winRect.y = 0;
		winRect.w = x; winRect.h = y;
		SDL_SetRenderDrawColor(anemoia::Locator::GetRenderer(), 0, 0, 0, Uint8(255.f * m_TransitionAlpha));
		SDL_RenderFillRect(anemoia::Locator::GetRenderer(), &winRect);
	}
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
		anemoia::Font* const pFont = anemoia::ResourceManager::GetInstance()->LoadFont("Fonts/Pixel_NES.otf", 24);

		//Create text holder
		anemoia::GameObject* const pRoot = new anemoia::GameObject(this);
		anemoia::TextComponent* pText1 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -100.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"NOW, IT IS BEGINNING OF A", pFont, SDL_Color{255, 0, 0});
		anemoia::TextComponent* pText2 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"FANTASTIC STORY!! LET'S MAKE A", pFont, SDL_Color{ 255, 0, 0 });
		anemoia::TextComponent* pText3 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 0.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"JOURNEY TO THE CAVE OF MONSTERS!", pFont, SDL_Color{ 255, 0, 0 });
		anemoia::TextComponent* pText4 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
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

	//P1
	{
		//Create gameobject
		m_pP1 = new anemoia::GameObject(this);

		//Create animated sprite comp
		anemoia::AnimSpriteComponent* const pAnimComp = new anemoia::AnimSpriteComponent(m_pP1,
			anemoia::Transform(glm::vec3{}, glm::vec2{ 0.5f, 0.5f }), "Anims/Intro_P1.txt", "Intro");
		m_pP1->AddComponent(pAnimComp);

		//Position
		m_pP1->SetPosition(0.3f * x, 0.65f * y, 0.f);

		//Add to scene
		AddChild(m_pP1);
	}

	//P2
	{
		//Create gameobject
		m_pP2 = new anemoia::GameObject(this);

		//Create animated sprite comp
		anemoia::AnimSpriteComponent* const pAnimComp = new anemoia::AnimSpriteComponent(m_pP2,
			anemoia::Transform(glm::vec3{}, glm::vec2{ 0.5f, 0.5f }), "Anims/Intro_P2.txt", "Intro");
		m_pP2->AddComponent(pAnimComp);

		//Position
		m_pP2->SetPosition(0.7f * x, 0.65f * y, 0.f);

		//Add to scene
		AddChild(m_pP2);
	}
}

void IntroScene::OnSceneActivated()
{
	//Scene comes into view
	m_TransitionAlphaTarget = 0.f;
	m_TransitionAlpha = 1.f;
}

void IntroScene::OnSceneDeactivated()
{
}

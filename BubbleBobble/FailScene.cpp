#include "pch.h"
#include "FailScene.h"

#include "InputManager.h"
#include "Command.h"
#include "BubbleBobbleGame.h"

#include "Font.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"

#include "AnimSpriteComponent.h"

FailScene::FailScene()
	: Scene("FailScene")
{
	//Inputs for starting the game
	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("sp", this, 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, anemoia::ButtonState::Down, [this]()
	{
		m_Pressed = true;
		m_QuitRequested = false;
		m_TransitionAlphaTarget = 1.f;
	}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("vs", this, 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, anemoia::ButtonState::Down, [this]()
	{
		m_Pressed = true;
		m_QuitRequested = true;
		m_TransitionAlphaTarget = 1.f;
	}));

	//Transition opacity
	m_TransitionAlpha = 1.f;
	m_TransitionAlphaTarget = 1.f;
	m_TransitionSpeed = 8.f;
}

FailScene::~FailScene()
{
}

void FailScene::FixedUpdate(float timeStep)
{
	//Call root
	Scene::FixedUpdate(timeStep);
}

void FailScene::Update(float elapsedSec)
{
	//Call root
	Scene::Update(elapsedSec);

	//Update transition opacity by lerping to target
	m_TransitionAlpha = m_TransitionAlpha + (m_TransitionAlphaTarget - m_TransitionAlpha) * elapsedSec * m_TransitionSpeed;

	if (m_Pressed && m_TransitionAlpha > 0.99f)
	{
		if (m_QuitRequested)
		{
			static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->Exit();
		}
		else
		{
			static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->Restart();
		}
	}
}

void FailScene::LateUpdate(float elapsedSec)
{
	//Call root
	Scene::LateUpdate(elapsedSec);
}

void FailScene::Render() const
{
	//Call root
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

void FailScene::Initialise()
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
			"You fail...", pFont, SDL_Color{ 255, 255, 255 });
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

	//Sprites
	{
		//Root
		anemoia::GameObject* const pRoot = new anemoia::GameObject(this);

		//Character
		anemoia::AnimSpriteComponent* const pAnim =
			new anemoia::AnimSpriteComponent(pRoot,
				anemoia::Transform(glm::vec3{ -128.f, -325.f, 0.f }, glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 2.f, 2.f }),
				"Anims/Fail_P1.txt", "Burn");
		pRoot->AddComponent(pAnim);

		//Character P2
		anemoia::AnimSpriteComponent* const pAnimP2 =
			new anemoia::AnimSpriteComponent(pRoot,
				anemoia::Transform(glm::vec3{ 128.f, -325.f, 0.f }, glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 2.f, 2.f }, 0.f, SDL_FLIP_HORIZONTAL),
				"Anims/Fail_P2.txt", "Burn");
		pRoot->AddComponent(pAnimP2);

		//Move whole
		pRoot->SetPosition(0.5f * x, 0.95f * y, 0.f);

		//Bind end of animation -> disappear
		pAnim->SetBoundFunction([pRoot]()
		{
			pRoot->SetEnabled(false);
		}, "Burn");

		//Add to scene
		AddChild(pRoot);
	}
}

void FailScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();

	//Scene comes into view
	m_TransitionAlphaTarget = 0.f;
	m_TransitionAlpha = 1.f;
}

void FailScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
}
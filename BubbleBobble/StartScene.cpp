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

#include "AnimSpriteComponent.h"
#include "Texture2D.h"
#include "Sound.h"

#include <iomanip>

StartScene::StartScene()
	: Scene("StartScene")
{
	//Inputs for starting the game
	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("sp", this, 0, XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::singleplayer);
		PlayStartEffects();
	}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("mp", this, 0, XINPUT_GAMEPAD_DPAD_UP, VK_UP, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::multiplayer);
		PlayStartEffects();
	}));

	anemoia::InputManager::GetInstance()->RegisterCommand(new anemoia::Command("vs", this, 0, XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT, anemoia::ButtonState::Down, [this]()
	{
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->SetGamemode(Gamemode::versus);
		PlayStartEffects();
	}));

	//Transition opacity
	m_TransitionAlpha = 1.f;
	m_TransitionAlphaTarget = 1.f;
	m_TransitionSpeed = 8.f;
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

	//Logo appears gradually
	m_pLogo->SetAlpha(glm::mix(m_pLogo->GetAlpha(), 255.f, 0.5f * elapsedSec));

	if (m_Start)
	{
		StartGame();
	}

	//Update transition opacity by lerping to target
	m_TransitionAlpha = m_TransitionAlpha + (m_TransitionAlphaTarget - m_TransitionAlpha) * elapsedSec * m_TransitionSpeed;
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
		anemoia::TextComponent* const pText1 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -100.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"<- for singleplayer", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* const pText2 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, -50.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"^ for multiplayer", pFont, SDL_Color{ 255, 255, 255 });
		anemoia::TextComponent* const pText3 = new anemoia::TextComponent(pRoot, anemoia::Transform{ glm::vec3{0.f, 0.f, 0.f}, glm::vec2{0.5f, 0.5f} },
			"-> for versus", pFont, SDL_Color{ 255, 255, 255 });

		//Add them
		pRoot->AddComponent(pText1);
		pRoot->AddComponent(pText2);
		pRoot->AddComponent(pText3);

		//Logo
		m_pLogo =
			new anemoia::AnimSpriteComponent(pRoot, anemoia::Transform(glm::vec3{ 0.f, -375.f, 0.f }, glm::vec2{ 0.5f, 0.5f }),
				"Anims/Logo.txt", "Anim", glm::vec4(255.f, 255.f, 255.f, 0.f));
		pRoot->AddComponent(m_pLogo);

		//Move whole
		pRoot->SetPosition(0.5f * x, 0.95f * y, 0.f);

		//Add to scene
		AddChild(pRoot);
	}

	//Load sound + play
	m_pSound_Intro = anemoia::ResourceManager::GetInstance()->LoadSound("HUD/Opening.wav");
	m_pSound_Select = anemoia::ResourceManager::GetInstance()->LoadSound("HUD/Select.wav");

	m_pSound_Intro->Play(0);
}

void StartScene::OnSceneActivated()
{
	//Call root
	Scene::OnSceneActivated();

	//Scene comes into view
	m_TransitionAlphaTarget = 0.f;
	m_TransitionAlpha = 1.f;

	//Tell controls
	PrintFormattedText("--- Controls ---", 0);
	PrintFormattedText("Gamepad:", 5);
	PrintFormattedText("D-PAD for movement, A to jump, X to attack", 10);
	PrintFormattedText("Keyboard:", 5);
	PrintFormattedText("Player 1: Arrows key for movement, Z to jump, X to attack", 10);
	PrintFormattedText("Player 2: NUMPAD4-6 for movement, NUMPAD8 to jump, NUMPAD5 to attack\n", 10);
}

void StartScene::OnSceneDeactivated()
{
	//Call root
	Scene::OnSceneDeactivated();
}

void StartScene::PrintFormattedText(const std::string& text, size_t spacing)
{
	const size_t totalSize = spacing + text.size();
	std::cout << std::setfill(' ') << std::setw(totalSize) << text << std::endl;
}

void StartScene::PlayStartEffects()
{
	if (!m_Start)
	{
		//Select
		m_pSound_Select->Play(0);
		m_pSound_Intro->Stop();

		//Transition
		m_TransitionAlphaTarget = 1.f;

		//Start has been requested
		m_Start = true;
	}
}

void StartScene::StartGame()
{
	//Add scenes
	if(m_TransitionAlpha > 0.99f)
	{
		//Game intro scene
		anemoia::Scene* const pIntro = new IntroScene();
		anemoia::SceneManager::GetInstance()->AddScene(pIntro);
		//anemoia::SceneManager::GetInstance()->SetActiveScene(pIntro);

		//Level 1 scene
		anemoia::Scene* const pLevel1 = new BaseGameScene(1, false);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel1);

		//Level 2 scene
		anemoia::Scene* const pLevel2 = new BaseGameScene(2, false);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel2);

		//Level 3 scene
		anemoia::Scene* const pLevel3 = new BaseGameScene(3, true);
		anemoia::SceneManager::GetInstance()->AddScene(pLevel3);

		//Go to intro
		anemoia::SceneManager::GetInstance()->SetActiveScene(pIntro);

		//Start music
		static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine())->PlayMusic(true);
	}
}

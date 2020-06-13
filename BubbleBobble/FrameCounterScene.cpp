#include "pch.h"
#include "FrameCounterScene.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "FPSComponent.h"

#include <SDL.h>

#include "Locator.h"

#include "Command.h"
#include <functional>

#include "Sound.h"

FrameCounterScene::FrameCounterScene()
	: Scene("FrameCounterScene")
{
}

FrameCounterScene::~FrameCounterScene()
{
}

void FrameCounterScene::FixedUpdate(float timeStep)
{
	//Call root fixed update
	Scene::FixedUpdate(timeStep);

	//Update text with timeStep -> we update the text on a regular basis
	m_pText->SetText(std::to_string( (int)m_pFPSComp->GetFPS()) + " FPS");

	//Get axis and set as text
	m_pAxisText->SetText(std::to_string(anemoia::Locator::GetInputManager()->GetTriggerAxis(0, false)));
	//m_pAxisText->SetText(std::to_string(anemoia::Locator::GetInputManager()->GetStickAxis(0, false, false))); //Try this out for both sticks
}

void FrameCounterScene::Update(float elapsedSec)
{
	//Call root update
	Scene::Update(elapsedSec);

	//Rotate
	anemoia::Transform transform = m_pText->GetTransform();
	transform.SetAngle(transform.GetAngle() + elapsedSec * 50.f);
	m_pText->SetTransform(transform);

	//Change alpha of background
	float alpha = m_pBackground->GetAlpha();
	alpha += elapsedSec * 100.f;
	if (alpha > 255.f)
	{
		alpha = 0.f;
	}
	m_pBackground->SetAlpha(alpha);
}

void FrameCounterScene::LateUpdate(float elapsedSec)
{
	//Call root late update
	Scene::LateUpdate(elapsedSec);
}

void FrameCounterScene::Render() const
{
	//Call root render
	Scene::Render();
}

void FrameCounterScene::Initialise()
{
	//Call root initialise
	Scene::Initialise();

	//Get window size to put object there
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Set up counter object
	anemoia::GameObject* const pCounter = new anemoia::GameObject();
	//pCounter->SetPosition(0.25f * x, 0.25f * y, 0.f); //Try this to show multiple transforms
	anemoia::Font* const pFont = anemoia::ResourceManager::GetInstance()->LoadFont("Pang.ttf", 36);

	//Create backdrop
	{
		const anemoia::Transform newTransform = anemoia::Transform(glm::vec3(0.5f * x, 0.5f * y, 0.f), glm::vec2(0.5f, 0.5f), glm::vec2(0.9f, 0.9f));
		anemoia::Texture2D* const pTex = anemoia::ResourceManager::GetInstance()->LoadTexture("background.jpg");
		m_pBackground = new anemoia::TextureComponent(pCounter, newTransform, pTex);
		pCounter->AddComponent(m_pBackground);
	}

	//Create text comp and add to object
	{
		const anemoia::Transform newTransform = anemoia::Transform(glm::vec3(0.5f * x, 0.5f * y, 0.f), glm::vec2(0.5f, 0.5f), glm::vec2(2.f, 2.f));
		m_pText = new anemoia::TextComponent(pCounter, newTransform, "0", pFont, SDL_Colour{ 255, 255, 0 });
		pCounter->AddComponent(m_pText);
	}

	//Create axis text comp and add to object
	{
		const anemoia::Transform newTransform = anemoia::Transform(glm::vec3(0.5f * x, 0.8f * y, 0.f), glm::vec2(0.5f, 0.f), glm::vec2(1.f, 1.f));
		m_pAxisText = new anemoia::TextComponent(pCounter, newTransform, "0", pFont, SDL_Colour{ 255, 255, 0 });
		pCounter->AddComponent(m_pAxisText);
	}

	//Create FPS comp and add to object
	{
		m_pFPSComp = new anemoia::FPSComponent(pCounter);
		pCounter->AddComponent(m_pFPSComp);
	}

	//Add to scene
	AddChild(pCounter);

	//Load playable sound
	m_pSound = anemoia::ResourceManager::GetInstance()->LoadSound("shoulder.mp3");

	//Add input
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput)
	{
		pInput->RegisterCommand(new anemoia::Command("RedText", this, 0, XINPUT_GAMEPAD_BACK, VK_RBUTTON, anemoia::ButtonState::Up, std::bind(&FrameCounterScene::ChangeTextToRed, this)));
		pInput->RegisterCommand(new anemoia::Command("BlueText", this, 0, XINPUT_GAMEPAD_START, VK_LBUTTON, anemoia::ButtonState::Hold, std::bind(&FrameCounterScene::ChangeTextToBlue, this)));
	}
}

void FrameCounterScene::OnSceneActivated()
{
}

void FrameCounterScene::OnSceneDeactivated()
{
}

void FrameCounterScene::ChangeTextToRed()
{
	m_pText->SetColour(SDL_Colour{ 255, 0, 0 });

	//Controller vibration test
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput) { pInput->SetControllerState(0, 1.f, 1.f); }

	//Sound test
	m_pSound->Play(0);
}

void FrameCounterScene::ChangeTextToBlue()
{
	m_pText->SetColour(SDL_Colour{ 0, 0, 255 });

	//Controller vibration test
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput) { pInput->SetControllerState(0, 0.f, 0.f); }
}
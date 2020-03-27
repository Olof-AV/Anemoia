#include "pch.h"
#include "FrameCounterScene.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "FPSComponent.h"

#include <SDL.h>

#include "Locator.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec4.hpp>
#pragma warning(pop)

#include "Command.h"
#include <functional>

#include "Sound.h"

FrameCounterScene::FrameCounterScene()
	: Scene("FrameCounterScene")
{
}

FrameCounterScene::~FrameCounterScene()
{
	//Delete commands associated to this scene
	delete m_pRed;
	delete m_pBlue;
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
	glm::vec4 colour = m_pBackground->GetColourMod();
	colour.a += elapsedSec * 100.f;
	if (colour.a > 255.f)
	{
		colour.a = 0.f;
	}
	m_pBackground->SetColourMod(colour);
}

void FrameCounterScene::LateUpdate(float elapsedSec)
{
	//Call root late update
	Scene::LateUpdate(elapsedSec);

	UNREFERENCED_PARAMETER(elapsedSec);
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

	m_pSound = anemoia::ResourceManager::GetInstance()->LoadSound("shoulder.mp3");
}

void FrameCounterScene::OnSceneActivated()
{
	//Add input if not present
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput)
	{
		if (!m_pRed)
		{
			m_pRed = new anemoia::Command("RedText", 0, XINPUT_GAMEPAD_BACK, VK_RBUTTON, anemoia::ButtonState::Up, std::bind(&FrameCounterScene::ChangeTextToRed, this));
		}
		if (!m_pBlue)
		{
			m_pBlue = new anemoia::Command("BlueText", 0, XINPUT_GAMEPAD_START, VK_LBUTTON, anemoia::ButtonState::Hold, std::bind(&FrameCounterScene::ChangeTextToBlue, this));
		}
		pInput->RegisterCommand(m_pRed);
		pInput->RegisterCommand(m_pBlue);
	}
}

void FrameCounterScene::OnSceneDeactivated()
{
	//Disable input
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput)
	{
		pInput->UnregisterCommand(m_pRed);
		pInput->UnregisterCommand(m_pBlue);
	}
}

void FrameCounterScene::ChangeTextToRed()
{
	m_pText->SetColour(SDL_Colour{ 255, 0, 0 });

	//Controller vibration test
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput) { pInput->SetControllerState(0, 65535, 65535); }

	//Sound test
	m_pSound->Play(0);
}

void FrameCounterScene::ChangeTextToBlue()
{
	m_pText->SetColour(SDL_Colour{ 0, 0, 255 });

	//Controller vibration test
	anemoia::InputManager* const pInput = anemoia::Locator::GetInputManager();
	if (pInput) { pInput->SetControllerState(0, 0, 0); }
}
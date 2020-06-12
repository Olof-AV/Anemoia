#include "pch.h"
#include "HUDComponent.h"

#include "Locator.h"
#include "BubbleBobbleGame.h"
#include "Texture2D.h"
#include "GameObject.h"

#include <SDL_ttf.h>
#include "Font.h"

HUDComponent::HUDComponent(anemoia::GameObject* const pParent, const anemoia::Transform& transform,
	anemoia::Font* const pFont, anemoia::Texture2D* const pHealthP1, anemoia::Texture2D* const pHealthP2)
	: anemoia::BaseComponent(pParent, transform), m_pFont{ pFont }, m_pTexHealthP1{ pHealthP1 }, m_pTexHealthP2{ pHealthP2 }
{
	m_pGameInstance = static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine());

	UpdateScores();
	UpdateText(m_pTextHiScore, "HIGH SCORE", SDL_Colour{ 255, 0, 0 });

	//Update initial texts already
	switch (m_pGameInstance->GetGamemode())
	{
	case Gamemode::singleplayer:
		UpdateText(m_pTextTopP1, "1UP", SDL_Colour{ 0, 255, 0 });
		UpdateText(m_pTextTopP2, "INSERT COIN", SDL_Colour{ 0, 187, 255 });

		break;

	case Gamemode::multiplayer:
		UpdateText(m_pTextTopP1, "1UP", SDL_Colour{ 0, 255, 0 });
		UpdateText(m_pTextTopP2, "2UP", SDL_Colour{ 0, 187, 255 });

		break;

	case Gamemode::versus:
		UpdateText(m_pTextTopP1, "1UP", SDL_Colour{ 0, 255, 0 });
		UpdateText(m_pTextTopP2, "VERSUS", SDL_Colour{ 0, 187, 255 });

		break;
	}
}

HUDComponent::~HUDComponent()
{
	delete m_pTextHiScore;
	delete m_pTextTopP1;
	delete m_pTextTopP2;

	delete m_pTextHiScoreNum;
	delete m_pTextScoreP1;
	delete m_pTextScoreP2;
}

void HUDComponent::FixedUpdate(float /*timeStep*/)
{

}

void HUDComponent::Update(float /*elapsedSec*/)
{

}

void HUDComponent::LateUpdate(float /*elapsedSec*/)
{

}

void HUDComponent::Render() const
{
	//Get dimensions
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);

	//Draw necessary
	switch (m_pGameInstance->GetGamemode())
	{
	case Gamemode::singleplayer:
		DrawHealth(true);

		break; 

	case Gamemode::multiplayer:
		DrawHealth(true);
		DrawHealth(false);

		break;

	case Gamemode::versus:
		DrawHealth(true);

		break;
	}

	//Common
	DrawTextUI(m_pTextHiScore, glm::vec2{ 0.5f, 0.015f });
	DrawTextUI(m_pTextTopP1, glm::vec2{ 0.2f, 0.015f });
	DrawTextUI(m_pTextTopP2, glm::vec2{ 0.8f, 0.015f });

	DrawTextUI(m_pTextHiScoreNum, glm::vec2{ 0.5f, 0.045f });
	DrawTextUI(m_pTextScoreP1, glm::vec2{ 0.2f, 0.045f });
	DrawTextUI(m_pTextScoreP2, glm::vec2{ 0.8f, 0.045f });
}

void HUDComponent::UpdateScores()
{
	//Updates scores exclusively
	UpdateText(m_pTextHiScoreNum, std::to_string(m_pGameInstance->GetHiScore()), SDL_Colour{ 255, 255, 255 });
	UpdateText(m_pTextScoreP1, std::to_string(m_pGameInstance->GetScore(true)), SDL_Colour{ 255, 255, 255 });
	UpdateText(m_pTextScoreP2, std::to_string(m_pGameInstance->GetScore(false)), SDL_Colour{ 255, 255, 255 });
}

void HUDComponent::UpdateText(anemoia::Texture2D* &pToUpdate, const std::string& newText, const SDL_Colour& colour)
{
	SDL_Surface* const pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), newText.c_str(), colour);
	if (!pSurface)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	const float x = (float)pSurface->w;
	const float y = (float)pSurface->h;

	SDL_Texture* const pTexture = SDL_CreateTextureFromSurface(anemoia::Locator::GetRenderer(), pSurface);
	if (!pTexture)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}

	//AFter you are done with a surface, free it
	SDL_FreeSurface(pSurface);

	//Create new texture 2D (managed by self)
	delete pToUpdate;
	pToUpdate = new anemoia::Texture2D(pTexture, glm::vec2(x, y));
}

void HUDComponent::DrawHealth(bool isP1) const
{
	//Get dimensions
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
	const glm::vec2 windowSize{ x * m_Transform.GetScale().x, y * m_Transform.GetScale().y };

	anemoia::Texture2D* const pTex = (isP1) ? m_pTexHealthP1 : m_pTexHealthP2;

	//Obtain global blend mode and set that as texture blend mode
	SDL_BlendMode blendMode;
	SDL_GetRenderDrawBlendMode(anemoia::Locator::GetRenderer(), &blendMode);
	SDL_SetTextureBlendMode(pTex->GetSDLTexture(), blendMode);

	//Calculate params
	const glm::vec2 size = pTex->GetDimensions() * m_Transform.GetScale();
	const glm::vec2 pivotOffset = (m_Transform.GetPivot() * windowSize);
	
	for (int i{}; i < m_pGameInstance->GetLives(isP1); ++i)
	{
		glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;
		finalPos.y += windowSize.y - size.y;
		if (isP1)
		{
			finalPos.x += size.x * i;
		}
		else
		{
			finalPos.x += windowSize.x - size.x;
			finalPos.x -= size.x * i;
		}

		//Render texture
		anemoia::Renderer::GetInstance()->RenderTexture(pTex, finalPos.x, finalPos.y, size.x, size.y,
			m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip());
	}
}

void HUDComponent::DrawTextUI(anemoia::Texture2D* pText, const glm::vec2& pos) const
{
	//Get dimensions
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
	const glm::vec2 windowSize{ x * m_Transform.GetScale().x, y * m_Transform.GetScale().y };

	//Obtain global blend mode and set that as texture blend mode
	SDL_BlendMode blendMode;
	SDL_GetRenderDrawBlendMode(anemoia::Locator::GetRenderer(), &blendMode);
	SDL_SetTextureBlendMode(pText->GetSDLTexture(), blendMode);

	//Calculate params
	const glm::vec2 size = pText->GetDimensions() * m_Transform.GetScale();
	const glm::vec2 pivotOffset = (m_Transform.GetPivot() * windowSize);

	glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;
	finalPos += pos * windowSize;
	finalPos -= size * 0.5f;

	//Render texture
	anemoia::Renderer::GetInstance()->RenderTexture(pText, finalPos.x, finalPos.y, size.x, size.y,
		m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip());
}
#include "pch.h"
#include "HUDComponent.h"

#include "Locator.h"
#include "BubbleBobbleGame.h"
#include "Texture2D.h"
#include "GameObject.h"

HUDComponent::HUDComponent(anemoia::GameObject* const pParent, const anemoia::Transform& transform,
	anemoia::Texture2D* const pHealthP1, anemoia::Texture2D* const pHealthP2)
	: anemoia::BaseComponent(pParent, transform), m_pHealthP1{pHealthP1}, m_pHealthP2{pHealthP2}
{
	m_pGameInstance = static_cast<BubbleBobbleGame*>(anemoia::Locator::GetEngine());
}

void HUDComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);

	//Update texts if needed
	switch (m_pGameInstance->GetGamemode())
	{
	case Gamemode::singleplayer:

		break;

	case Gamemode::multiplayer:

		break;

	case Gamemode::versus:

		break;
	}
}

void HUDComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void HUDComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
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
		DrawHealth(false);
		break; 

	case Gamemode::multiplayer:

		break;

	case Gamemode::versus:

		break;
	}
}

void HUDComponent::DrawHealth(bool isP1) const
{
	//Get dimensions
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
	const glm::vec2 windowSize{ x * m_Transform.GetScale().x, y * m_Transform.GetScale().y };

	anemoia::Texture2D* pTex = (isP1) ? m_pHealthP1 : m_pHealthP2;

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
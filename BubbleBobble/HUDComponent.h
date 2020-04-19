#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class GameInstance;
	class Font;
}

class BubbleBobbleGame;

class HUDComponent final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	HUDComponent(anemoia::GameObject* const pParent, const anemoia::Transform& transform,
		anemoia::Font* const pFont, anemoia::Texture2D* const pHealthP1, anemoia::Texture2D* const pHealthP2);
	virtual ~HUDComponent();

	HUDComponent(const HUDComponent& other) = delete;
	HUDComponent(HUDComponent&& other) = delete;
	HUDComponent& operator=(const HUDComponent& other) = delete;
	HUDComponent& operator=(HUDComponent&& other) = delete;
#pragma endregion Constructors

	//Overrides
	virtual void FixedUpdate(float timeStep) override;
	virtual void Update(float elapsedSec) override;
	virtual void LateUpdate(float elapsedSec) override;
	virtual void Render() const override;

	//Functions
	void UpdateScores();

private:
	void UpdateText(anemoia::Texture2D* &pToUpdate, const std::string& newText, const SDL_Colour& colour);

	void DrawHealth(bool isP1) const;
	void DrawTextUI(anemoia::Texture2D* pText, const glm::vec2& pos) const;
	
	BubbleBobbleGame* m_pGameInstance = nullptr;

	anemoia::Texture2D* m_pTexHealthP1 = nullptr;
	anemoia::Texture2D* m_pTexHealthP2 = nullptr;
	anemoia::Font* m_pFont = nullptr;

	//Text
	anemoia::Texture2D* m_pTextHiScore = nullptr;
	anemoia::Texture2D* m_pTextTopP1 = nullptr;
	anemoia::Texture2D* m_pTextTopP2 = nullptr;

	anemoia::Texture2D* m_pTextHiScoreNum = nullptr;
	anemoia::Texture2D* m_pTextScoreP1 = nullptr;
	anemoia::Texture2D* m_pTextScoreP2 = nullptr;
};
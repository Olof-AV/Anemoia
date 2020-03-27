#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class GameInstance;
}

class BubbleBobbleGame;

class HUDComponent final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	HUDComponent(anemoia::GameObject* const pParent, const anemoia::Transform& transform,
		anemoia::Texture2D* const pHealthP1, anemoia::Texture2D* const pHealthP2);
	virtual ~HUDComponent() = default;

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

private:
	void DrawHealth(bool isP1) const;
	
	BubbleBobbleGame* m_pGameInstance = nullptr;

	anemoia::Texture2D* m_pHealthP1 = nullptr;
	anemoia::Texture2D* m_pHealthP2 = nullptr;
};
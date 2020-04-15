#pragma once
#include "Scene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
	class GameObject;
	class Texture2D;
}

class BaseGameScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	BaseGameScene(UINT levelNum);
	virtual ~BaseGameScene();

	BaseGameScene(const BaseGameScene& other) = delete;
	BaseGameScene(BaseGameScene&& other) = delete;
	BaseGameScene& operator=(const BaseGameScene& other) = delete;
	BaseGameScene& operator=(BaseGameScene&& other) = delete;
#pragma endregion Constructors

	//Overrides
	virtual void FixedUpdate(float timeStep) override;
	virtual void Update(float elapsedSec) override;
	virtual void LateUpdate(float elapsedSec) override;
	virtual void Render() const override;

	//Scene stuff
	virtual void Initialise() override;
	virtual void OnSceneActivated() override;
	virtual void OnSceneDeactivated() override;

protected:
	void ReadLevelData();

	bool CheckDataForFakeTile(const std::string& input);
	bool CheckDataForTile(const std::string& input);
	bool CheckDataForBigTile(const std::string& input);

	void CreateTile(const glm::vec2& pos, bool isBig, bool hasCollision = true);

	anemoia::GameObject* m_pHUD = nullptr;
	UINT m_LevelNum = 0;
};
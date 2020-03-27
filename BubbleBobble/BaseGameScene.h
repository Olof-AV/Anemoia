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

class BaseGameScene : public anemoia::Scene
{
public:
#pragma region Constructors
	BaseGameScene(const std::string &name);
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
	anemoia::GameObject* m_pHUD = nullptr;
};
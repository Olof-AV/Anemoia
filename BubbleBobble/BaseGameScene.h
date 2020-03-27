#pragma once
#include "Scene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
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
	virtual void FixedUpdate(float timeStep);
	virtual void Update(float elapsedSec);
	virtual void LateUpdate(float elapsedSec);
	virtual void Render() const;

	//Scene stuff
	virtual void Initialise();
};
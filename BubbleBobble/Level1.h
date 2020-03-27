#pragma once
#include "BaseGameScene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
}

class Level1 final : public BaseGameScene
{
public:
#pragma region Constructors
	Level1();
	virtual ~Level1();

	Level1(const Level1& other) = delete;
	Level1(Level1&& other) = delete;
	Level1& operator=(const Level1& other) = delete;
	Level1& operator=(Level1&& other) = delete;
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

private:

};
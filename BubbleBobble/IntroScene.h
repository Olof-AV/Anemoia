#pragma once
#include "Scene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
}

class IntroScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	IntroScene();
	virtual ~IntroScene();

	IntroScene(const IntroScene& other) = delete;
	IntroScene(IntroScene&& other) = delete;
	IntroScene& operator=(const IntroScene& other) = delete;
	IntroScene& operator=(IntroScene&& other) = delete;
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
	const float m_MaxTime = 5.f;
	float m_CurrentTime = 0.f;
	anemoia::Sound* m_pSound = nullptr;
};
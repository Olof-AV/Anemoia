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
	virtual void FixedUpdate(float timeStep);
	virtual void Update(float elapsedSec);
	virtual void LateUpdate(float elapsedSec);
	virtual void Render() const;

	//Scene stuff
	virtual void Initialise();

private:
	const float m_MaxTime = 5.f;
	float m_CurrentTime = 0.f;
	anemoia::Sound* m_pSound = nullptr;
};
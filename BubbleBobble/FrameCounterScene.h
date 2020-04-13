#pragma once
#include "Scene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
}

class FrameCounterScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	FrameCounterScene();
	virtual ~FrameCounterScene();

	FrameCounterScene(const Scene& other) = delete;
	FrameCounterScene(FrameCounterScene&& other) = delete;
	FrameCounterScene& operator=(const FrameCounterScene& other) = delete;
	FrameCounterScene& operator=(FrameCounterScene&& other) = delete;
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
	void ChangeTextToRed();
	void ChangeTextToBlue();

	anemoia::TextComponent* m_pAxisText = nullptr;
	anemoia::TextComponent* m_pText = nullptr;
	anemoia::TextureComponent* m_pBackground = nullptr;
	anemoia::FPSComponent* m_pFPSComp = nullptr;

	anemoia::Sound* m_pSound = nullptr;
};
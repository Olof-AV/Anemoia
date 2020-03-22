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
	virtual void FixedUpdate(float timeStep);
	virtual void Update(float elapsedSec);
	virtual void Render() const;

	//Scene stuff
	virtual void Initialise();

private:
	void ChangeTextToRed();
	void ChangeTextToBlue();

	anemoia::TextComponent* m_pAxisText = nullptr;
	anemoia::TextComponent* m_pText = nullptr;
	anemoia::TextureComponent* m_pBackground = nullptr;
	anemoia::FPSComponent* m_pFPSComp = nullptr;

	anemoia::Sound* m_pSound = nullptr;
};
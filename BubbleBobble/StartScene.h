#pragma once
#include "Scene.h"

namespace anemoia
{
	class AnimSpriteComponent;
	class Sound;
}

class StartScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	StartScene();
	virtual ~StartScene();

	StartScene(const StartScene& other) = delete;
	StartScene(StartScene&& other) = delete;
	StartScene& operator=(const StartScene& other) = delete;
	StartScene& operator=(StartScene&& other) = delete;
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
	void PrintFormattedText(const std::string& text, size_t spacing);

	void PlayStartEffects();
	void StartGame();
	bool m_Start = false;

	anemoia::AnimSpriteComponent* m_pLogo = nullptr;
	anemoia::Sound* m_pSound_Intro = nullptr;
	anemoia::Sound* m_pSound_Select = nullptr;

	//Transition
	float m_TransitionAlpha;
	float m_TransitionAlphaTarget;
	float m_TransitionSpeed;
};


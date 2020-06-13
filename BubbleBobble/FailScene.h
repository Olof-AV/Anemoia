#pragma once
#include "Scene.h"

class FailScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	FailScene();
	virtual ~FailScene();

	FailScene(const FailScene& other) = delete;
	FailScene(FailScene&& other) = delete;
	FailScene& operator=(const FailScene& other) = delete;
	FailScene& operator=(FailScene&& other) = delete;
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
	bool m_Pressed = false;
	bool m_QuitRequested = false;

	//Transition
	float m_TransitionAlpha;
	float m_TransitionAlphaTarget;
	float m_TransitionSpeed;
};
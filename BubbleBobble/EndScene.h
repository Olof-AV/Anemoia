#pragma once
#include "Scene.h"

class EndScene final : public anemoia::Scene
{
public:
#pragma region Constructors
	EndScene();
	virtual ~EndScene();

	EndScene(const EndScene& other) = delete;
	EndScene(EndScene&& other) = delete;
	EndScene& operator=(const EndScene& other) = delete;
	EndScene& operator=(EndScene&& other) = delete;
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
	//Transition
	float m_TransitionAlpha;
	float m_TransitionAlphaTarget;
	float m_TransitionSpeed;
};
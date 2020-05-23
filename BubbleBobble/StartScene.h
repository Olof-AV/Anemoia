#pragma once
#include "Scene.h"

namespace anemoia
{
	class TextureComponent;
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
	void StartGame();
	bool m_Start = false;

	anemoia::TextureComponent* m_pLogo = nullptr;
	anemoia::Sound* m_pSound_Intro = nullptr;
	anemoia::Sound* m_pSound_Select = nullptr;
};


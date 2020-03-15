#pragma once
#include "Scene.h"

namespace anemoia
{
	class FrameCounterScene final : public Scene
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
		class TextComponent* m_pText = nullptr;
		class FPSComponent* m_pFPSComp = nullptr;
	};
}
#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class FPSComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		FPSComponent(GameObject* const pParent);
		virtual ~FPSComponent() = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void LateUpdate(float elapsedSec) override;
		virtual void Render() const override {};

		//Doesn't hold a transform
#pragma region Delete
		const Transform& GetTransform() const = delete;
		void SetTransform(const Transform& newTransform) = delete;
#pragma region Delete

		//Functions
		float GetFPS() const;

	private:
		float m_FPS;
	};
}
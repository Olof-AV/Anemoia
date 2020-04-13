#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class PlayerControllerComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		PlayerControllerComponent(GameObject* const pParent, const Transform& transform);
		virtual ~PlayerControllerComponent() = default;

		PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
		PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;
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

	private:
		void Move(float timeStep, const glm::vec2& dir);

		glm::vec2 m_InputDir;
	};
}
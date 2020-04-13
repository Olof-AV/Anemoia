#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class ColliderComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		ColliderComponent(GameObject* const pParent, const Transform& transform, const glm::vec2 &hitbox);
		virtual ~ColliderComponent() = default;

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void LateUpdate(float elapsedSec) override;
		virtual void Render() const override;

	private:
		glm::vec2 m_Hitbox;
	};
}
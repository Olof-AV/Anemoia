#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class ColliderComponent;

	class RigidBodyComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		RigidBodyComponent(GameObject* const pParent, ColliderComponent* const pColl);
		virtual ~RigidBodyComponent() = default;

		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void LateUpdate(float elapsedSec) override;
		virtual void Render() const override;

		//Doesn't hold a transform
#pragma region Delete
		const Transform& GetTransform() const = delete;
		void SetTransform(const Transform & newTransform) = delete;
#pragma region Delete

	private:
		ColliderComponent* m_pLinkedCollider;

		float m_Gravity = 800.f;
		glm::vec2 m_Velocity{};
	};
}

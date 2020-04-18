#pragma once
#include "BaseComponent.h"
#include <unordered_set>

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

		//Functions
		void Move(const glm::vec2& newPos);
		void AddVelocity(const glm::vec2 &value);
		void SetVelocity(const glm::vec2& value);
		const glm::vec2 &GetVelocity() const;
		bool IsTouchingFloor() const;

#pragma region Tags
		bool AddIgnoreTag(const std::string& tag);
		bool RemoveIgnoreTag(const std::string& tag);
#pragma endregion Tags

	private:
		void CheckCollision();
		void CheckOutOfBounds();

		std::unordered_set<std::string> m_IgnoreTags;

		ColliderComponent* m_pLinkedCollider;

		float m_Gravity = 1400.f;
		glm::vec2 m_Velocity{};

		bool m_IsTouchingFloor = false;
	};
}

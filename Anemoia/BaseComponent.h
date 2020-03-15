#pragma once
#include "Transform.h"

namespace anemoia
{
	class GameObject;

	class BaseComponent
	{
	public:
#pragma region Constructors
		BaseComponent(GameObject* const pParent,
			const Transform& transform);

		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;
#pragma endregion Constructors

		//Actual functions
		virtual void FixedUpdate(float timeStep) = 0;
		virtual void Update(float elapsedSec) = 0;
		virtual void Render() const = 0;

#pragma region GettersSetters
		//Transforms
		const Transform& GetTransform() const;
		void SetTransform(const Transform& newTransform);

		//Parent related functions
		GameObject* const GetParent() const;
#pragma endregion GettersSetters

	protected:
		//Base
		Transform m_Transform;
		GameObject* m_pParent;
	};
}
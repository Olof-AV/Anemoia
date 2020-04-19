#pragma once
#include "BaseComponent.h"
#include "Event.h"

namespace anemoia
{
	class RigidBodyComponent;
	class GameObject;
}

class ItemBehaviour final : public anemoia::BaseComponent
{
#pragma region Constructors
	ItemBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::Events attachedEvent);
	virtual ~ItemBehaviour() = default;

	ItemBehaviour(const ItemBehaviour& other) = delete;
	ItemBehaviour(ItemBehaviour&& other) = delete;
	ItemBehaviour& operator=(const ItemBehaviour& other) = delete;
	ItemBehaviour& operator=(ItemBehaviour&& other) = delete;
#pragma endregion Constructors

	//Overrides
	virtual void FixedUpdate(float timeStep) override;
	virtual void Update(float elapsedSec) override;
	virtual void LateUpdate(float elapsedSec) override;
	virtual void Render() const override {};

	virtual void OnCollide(anemoia::GameObject* const pOther);

	//Doesn't hold a transform
#pragma region Delete
	const anemoia::Transform& GetTransform() const = delete;
	void SetTransform(const anemoia::Transform & newTransform) = delete;
#pragma endregion Delete

private:

};


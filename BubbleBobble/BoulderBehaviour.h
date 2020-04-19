#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class TextureComponent;
	class GameObject;
}

class BoulderBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	BoulderBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool movesLeft);
	virtual ~BoulderBehaviour() = default;

	BoulderBehaviour(const BoulderBehaviour& other) = delete;
	BoulderBehaviour(BoulderBehaviour&& other) = delete;
	BoulderBehaviour& operator=(const BoulderBehaviour& other) = delete;
	BoulderBehaviour& operator=(BoulderBehaviour&& other) = delete;
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
	void SetTransform(const anemoia::Transform& newTransform) = delete;
#pragma endregion Delete

private:
	glm::vec2 m_Movement;
	bool m_MovesLeft;

	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::TextureComponent* m_pTexComp;

	anemoia::Texture2D* m_pTexBoulder;
};

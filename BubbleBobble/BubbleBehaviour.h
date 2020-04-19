#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class TextureComponent;
	class GameObject;
}

class BubbleBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	BubbleBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool movesLeft);
	virtual ~BubbleBehaviour() = default;

	BubbleBehaviour(const BubbleBehaviour& other) = delete;
	BubbleBehaviour(BubbleBehaviour&& other) = delete;
	BubbleBehaviour& operator=(const BubbleBehaviour& other) = delete;
	BubbleBehaviour& operator=(BubbleBehaviour&& other) = delete;
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
#pragma region Delete

private:
	float m_BurstTimer;
	float m_BurstTimerMax;

	glm::vec2 m_Movement;
	float m_SlowDownRate;
	bool m_MovesLeft;

	float m_HorThreshold;

	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::TextureComponent* m_pTexComp;

	anemoia::Texture2D* m_pTexBubble;
};


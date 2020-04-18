#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class TextureComponent;
	class GameObject;
}

class ZenBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	ZenBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp);
	virtual ~ZenBehaviour() = default;

	ZenBehaviour(const ZenBehaviour& other) = delete;
	ZenBehaviour(ZenBehaviour&& other) = delete;
	ZenBehaviour& operator=(const ZenBehaviour& other) = delete;
	ZenBehaviour& operator=(ZenBehaviour&& other) = delete;
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
#pragma region Delete

private:
	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::TextureComponent* m_pTexComp;

	anemoia::Texture2D* m_pTexIdle;

	anemoia::GameObject* m_pPlayer;

	glm::vec2 m_InputDir;

	float m_MovSpeed;
	float m_JumpForce;
	float m_Threshold;

	bool m_IsDead;
};


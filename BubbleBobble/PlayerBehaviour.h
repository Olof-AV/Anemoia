#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class TextureComponent;
	class GameObject;
}

enum class PlayerState
{
	idle,
	shoot
};

class PlayerBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	PlayerBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool isP2 = false);
	virtual ~PlayerBehaviour() = default;

	PlayerBehaviour(const PlayerBehaviour& other) = delete;
	PlayerBehaviour(PlayerBehaviour&& other) = delete;
	PlayerBehaviour& operator=(const PlayerBehaviour& other) = delete;
	PlayerBehaviour& operator=(PlayerBehaviour&& other) = delete;
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

	//Functions
	void HandleMovement();
	void HandleInvincibilityTimer(float elapsedSec);

	void Die();
	void ShootBubble();

	PlayerState GetState() const;
	void SetState(PlayerState newState);

private:
	//Components
	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::TextureComponent* m_pTexComp;

	//State
	anemoia::Texture2D* m_pTexIdle;
	anemoia::Texture2D* m_pTexShoot;
	PlayerState m_CurrentState;

	//Movement
	glm::vec2 m_InitialPos;
	glm::vec2 m_InputDir;
	float m_MovSpeed;
	float m_JumpForce;

	//Death stuff
	bool m_IsDead;
	bool m_IsInvincible;
	float m_InvincibilityTimer;
	float m_InvincibilityTimerMax;

	//Shoot stuff
	float m_ShootCoolTimer;
	float m_ShootCoolTimerMax;
};


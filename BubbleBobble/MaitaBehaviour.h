#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class TextureComponent;
	class GameObject;
	class Command;
}

enum class MaitaState
{
	run,
	bubble
};

class MaitaBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	MaitaBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::TextureComponent* const pTexComp, bool isPlayer);
	virtual ~MaitaBehaviour();

	MaitaBehaviour(const MaitaBehaviour& other) = delete;
	MaitaBehaviour(MaitaBehaviour&& other) = delete;
	MaitaBehaviour& operator=(const MaitaBehaviour& other) = delete;
	MaitaBehaviour& operator=(MaitaBehaviour&& other) = delete;
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

	//Functions
	MaitaState GetState() const;
	void SetState(MaitaState newState);

	void HandleMovement();
	void HandleBubbleMov();

	void PlayerTouch(anemoia::GameObject* const pOther);

	void ShootBoulder();

	void RunAI();

#pragma region Delete

private:
	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::TextureComponent* m_pTexComp;

	anemoia::Texture2D* m_pTexRun;
	anemoia::Texture2D* m_pTexBubble;
	MaitaState m_CurrentState;

	anemoia::GameObject* m_pPlayer;

	glm::vec2 m_InputDir;

	float m_MovSpeed;
	float m_JumpForce;
	float m_HorThreshold;
	float m_VerThreshold;

	float m_FloatRate;
	float m_BubbleBurstTimer;
	float m_BubbleBurstTimerMax;

	bool m_BoulderCooldown;
	float m_BoulderTimer;
	float m_BoulderTimerMax;

	bool m_ShootRequested = false;

	bool m_IsDead;

	bool m_IsPlayer;

	//Commands
	anemoia::Command* m_pCommand_Left;
	anemoia::Command* m_pCommand_Right;
	anemoia::Command* m_pCommand_Jump;
	anemoia::Command* m_pCommand_Shoot;
};
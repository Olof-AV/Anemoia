#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class AnimSpriteComponent;
	class GameObject;
	class Command;
	class Sound;
}

enum class MaitaState
{
	run,
	attack,
	bubble,
	dying
};

class MaitaBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	MaitaBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp, bool isPlayer);
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

	void PrepareBoulder();
	void ShootBoulder();

	void RunAI();

	void GetBubbled(bool isP1);

#pragma region Delete

private:
	void SpawnItem();

	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::AnimSpriteComponent* m_pAnimComp;

	MaitaState m_CurrentState;

	std::vector<anemoia::GameObject*> m_Targets;

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

	bool m_IsPlayer;

	//Sounds
	anemoia::Sound* m_pSound_Death = nullptr;

	//Commands
	anemoia::Command* m_pCommand_Left = nullptr;
	anemoia::Command* m_pCommand_Right = nullptr;
	anemoia::Command* m_pCommand_Jump = nullptr;
	anemoia::Command* m_pCommand_Shoot = nullptr;
};
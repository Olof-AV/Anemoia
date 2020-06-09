#pragma once
#include "BaseComponent.h"

namespace anemoia
{
	class Texture2D;
	class RigidBodyComponent;
	class AnimSpriteComponent;
	class GameObject;
}

enum class ZenState
{
	run,
	bubble
};

class ZenBehaviour final : public anemoia::BaseComponent
{
public:
#pragma region Constructors
	ZenBehaviour(anemoia::GameObject* const pParent, anemoia::RigidBodyComponent* const pRigid, anemoia::AnimSpriteComponent* const pAnimComp);
	virtual ~ZenBehaviour();

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
#pragma endregion Delete

	//Functions
	ZenState GetState() const;
	void SetState(ZenState newState);

	void HandleMovement();
	void HandleBubbleMov();

	void PlayerTouch(anemoia::GameObject* const pOther);

	void RunAI();

	void GetBubbled(bool isP1);

#pragma region Delete

private:
	anemoia::RigidBodyComponent* m_pRigid;
	anemoia::AnimSpriteComponent* m_pAnimComp;

	ZenState m_CurrentState;

	std::vector<anemoia::GameObject*> m_Targets;

	glm::vec2 m_InputDir;

	float m_MovSpeed;
	float m_JumpForce;
	float m_HorThreshold;
	float m_VerThreshold;

	float m_FloatRate;
	float m_BubbleBurstTimer;
	float m_BubbleBurstTimerMax;

	bool m_IsDead;
};


#pragma once
#include "BaseComponent.h"

#include <unordered_map>
#include <functional>

namespace anemoia
{
	class Texture2D;

	struct AnimSprite
	{
		Texture2D* pTexture = nullptr;

		//Anim sprite
		int frameCount = -1;
		int nrCols = -1;
		int nrRows = -1;
		float framesPerSec = -1.f;
		float totalTime = -1.f;
		bool isPong = false;
		bool isReverse = false; //If pong is used, determines when it's going backwards or not

		std::function<void(void)> boundFunc; //Callback when animation ends
	};

	class AnimSpriteComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		AnimSpriteComponent(GameObject* const pParent, const Transform& transform,
			const std::string &animPath, const std::string &defaultAnim, const glm::vec4& colour = glm::vec4{ 255.f, 255.f, 255.f, 255.f });

		virtual ~AnimSpriteComponent() = default;

		AnimSpriteComponent(const AnimSpriteComponent& other) = delete;
		AnimSpriteComponent(AnimSpriteComponent&& other) = delete;
		AnimSpriteComponent& operator=(const AnimSpriteComponent& other) = delete;
		AnimSpriteComponent& operator=(AnimSpriteComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void LateUpdate(float elapsedSec) override;
		virtual void Render() const override;

#pragma region GettersSetters
		const std::string &GetAnim() const;
		void SetAnim(const std::string& name);

		const glm::vec4& GetColourMod() const;
		void SetColourMod(const glm::vec4& colour);

		float GetAlpha() const;
		void SetAlpha(float value);
#pragma endregion GettersSetters

		//Extra functions
		void ResetCurrentAnim();
		void SetBoundFunction(const std::function<void(void)>& func, const std::string& animName);

	private:
		glm::vec4 m_ColourMod;
		std::string m_ActiveSprite;

		std::unordered_map<std::string, AnimSprite> m_Sprites;
	};
}
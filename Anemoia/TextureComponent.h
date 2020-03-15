#pragma once
#include "BaseComponent.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec4.hpp>
#pragma warning(pop)

namespace anemoia
{
	class Texture2D;

	class TextureComponent final : public BaseComponent
	{
	public:
#pragma region Constructors
		TextureComponent(GameObject* const pParent, const Transform& transform,
			Texture2D* const pTexture, const glm::vec4& colour = glm::vec4{255.f, 255.f, 255.f, 255.f});

		virtual ~TextureComponent() = default;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
#pragma endregion Constructors

		//Overrides
		virtual void FixedUpdate(float timeStep) override;
		virtual void Update(float elapsedSec) override;
		virtual void Render() const override;

#pragma region GettersSetters
		Texture2D* const GetTexture() const;
		void SetTexture(Texture2D* const pTexture);

		const glm::vec4 &GetColourMod() const;
		void SetColourMod(const glm::vec4& colour);
#pragma endregion GettersSetters

	private:
		Texture2D* m_pTexture;
		glm::vec4 m_ColourMod;
	};
}
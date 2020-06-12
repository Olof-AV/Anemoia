#pragma once

#include <SDL_render.h>

namespace anemoia
{
	class Transform final
	{
	public:
#pragma warning(suppress:26812)
		Transform(const glm::vec3& pos = glm::vec3(0.f, 0.f, 0.f),
			const glm::vec2& pivot = glm::vec2(0.f, 0.f),
			const glm::vec2& scale = glm::vec2(1.f, 1.f),
			float angle = 0.f,
			SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE);

#pragma region GettersSetters
		const glm::vec3& GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& newPos);

		const glm::vec2& GetPivot() const;
		void SetPivot(const glm::vec2& pivot);
		void SetPivot(float x, float y);

		const glm::vec2& GetScale() const;
		void SetScale(const glm::vec2& scale);
		void SetScale(float x, float y);

		float GetAngle() const;
		void SetAngle(float angle);

		SDL_RendererFlip GetFlip() const;
		void SetFlip(SDL_RendererFlip flip);
#pragma endregion GettersSetters

	private:
		glm::vec3 m_Position;

		//Extra transforms applied on top of everything else
		glm::vec2 m_Pivot;
		glm::vec2 m_Scale;
		float m_Angle;
		SDL_RendererFlip m_Flip;
	};
}
#include "AnemoiaPCH.h"
#include "Transform.h"

#pragma warning(suppress:26812)
anemoia::Transform::Transform(const glm::vec3& pos,
	const glm::vec2& pivot,
	const glm::vec2& scale,
	float angle,
	SDL_RendererFlip flip)
	: m_Position{ pos }, m_Pivot{ pivot }, m_Scale{ scale }, m_Angle{ angle }, m_Flip{ flip }
{
}


#pragma region GettersSetters
const glm::vec3& anemoia::Transform::GetPosition() const
{
	return m_Position;
}

void anemoia::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void anemoia::Transform::SetPosition(const glm::vec3& newPos)
{
	m_Position = newPos;
}

const glm::vec2& anemoia::Transform::GetPivot() const
{
	return m_Pivot;
}

void anemoia::Transform::SetPivot(const glm::vec2& pivot)
{
	m_Pivot = pivot;
}

void anemoia::Transform::SetPivot(float x, float y)
{
	m_Pivot.x = x;
	m_Pivot.y = y;
}

const glm::vec2& anemoia::Transform::GetScale() const
{
	return m_Scale;
}

void anemoia::Transform::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
}

void anemoia::Transform::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}
float anemoia::Transform::GetAngle() const
{
	return m_Angle;
}
void anemoia::Transform::SetAngle(float angle)
{
	m_Angle = angle;
}
SDL_RendererFlip anemoia::Transform::GetFlip() const
{
	return m_Flip;
}
void anemoia::Transform::SetFlip(SDL_RendererFlip flip)
{
	m_Flip = flip;
}
#pragma endregion GettersSetters
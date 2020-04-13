#include "AnemoiaPCH.h"
#include "ColliderComponent.h"

#include "Locator.h"

anemoia::ColliderComponent::ColliderComponent(GameObject* const pParent, const Transform& transform, const glm::vec2 &hitbox)
	: BaseComponent(pParent, transform), m_Hitbox{ hitbox }
{
}

void anemoia::ColliderComponent::FixedUpdate(float timeStep)
{
}

void anemoia::ColliderComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::ColliderComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::ColliderComponent::Render() const
{
	//Get renderer
	SDL_Renderer* const pRenderer = Locator::GetRenderer();

	//Draws hitbox
	SDL_Rect rect;
	const glm::vec2 pivot = m_Transform.GetPivot();
	const glm::vec2 pos = m_Transform.GetPosition();
	const glm::vec2 scale = m_Transform.GetScale();

	//Set rect width/height
	rect.w = m_Hitbox.x * scale.x; rect.h = m_Hitbox.y * scale.y;

	//Set pos
	rect.x = pos.x - (pivot.x * rect.w); rect.y = pos.y - (pivot.y * rect.h);
	
	//Draw rect
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(pRenderer, &rect);
}

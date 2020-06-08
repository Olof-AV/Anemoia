#include "AnemoiaPCH.h"
#include "ColliderComponent.h"

#include "Locator.h"
#include "GameObject.h"
#include "Scene.h"

anemoia::ColliderComponent::ColliderComponent(GameObject* const pParent, const Transform& transform, const glm::vec2& hitbox, bool isImportant, bool affectRigids)
	: BaseComponent(pParent, transform), m_Hitbox{ hitbox }, m_IsImportant{ isImportant }, m_AffectRigids{ affectRigids }
{
	pParent->GetParentScene()->AddCollider(this);
}

anemoia::ColliderComponent::~ColliderComponent()
{
	//Don't forget to remove
	m_pParent->GetParentScene()->RemoveCollider(this);
}

void anemoia::ColliderComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
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
#if _DEBUG
	//Get renderer
	SDL_Renderer* const pRenderer = Locator::GetRenderer();

	//Draw rect representing hitbox
	SDL_Rect rect = GetRect();
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(pRenderer, &rect);
#endif
}

SDL_Rect anemoia::ColliderComponent::GetRect() const
{
	//Functions creates a rect with appropriate coordinates
	SDL_Rect rect;
	const glm::vec2 pivot = m_Transform.GetPivot();
	const glm::vec2 pos = m_Transform.GetPosition();
	const glm::vec3 objPos = GetParent()->GetPosition();
	const glm::vec2 scale = m_Transform.GetScale();

	//Set rect width/height
	rect.w = int(m_Hitbox.x * scale.x);
	rect.h = int(m_Hitbox.y * scale.y);

	//Set pos
	rect.x = int(objPos.x + pos.x - (pivot.x * rect.w));
	rect.y = int(objPos.y + pos.y - (pivot.y * rect.h));

	//Good to go
	return rect;
}

bool anemoia::ColliderComponent::IsImportant() const
{
	return m_IsImportant;
}

bool anemoia::ColliderComponent::AffectRigids() const
{
	return m_AffectRigids;
}

#include "AnemoiaPCH.h"
#include "RigidBodyComponent.h"

#include "Locator.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"

anemoia::RigidBodyComponent::RigidBodyComponent(GameObject* const pParent, ColliderComponent* const pColl)
	: BaseComponent(pParent, Transform()), m_pLinkedCollider{ pColl }
{
}

void anemoia::RigidBodyComponent::FixedUpdate(float timeStep)
{
	//For later
	glm::vec3 pos = GetParent()->GetPosition();

	//Apply gravity
	m_Velocity.y += m_Gravity * timeStep;

	//Apply velocity, test for collisions
	pos += glm::vec3(m_Velocity, 0.f) * timeStep;

	//Collision
	SDL_Rect rect = m_pLinkedCollider->GetRect();
	std::vector<ColliderComponent*> colliders = GetParent()->GetParentScene()->GetColliders();
	colliders.erase(std::find(colliders.cbegin(), colliders.cend(), m_pLinkedCollider)); //NO SELF COLLIDE

	//BOTTOM COLLISION
	{
		int x1 = int(rect.x + rect.w * 0.5f);
		int y1 = int(rect.y + rect.h * 0.5f);
		int x2 = x1;
		int y2 = int(rect.y + rect.h);

		bool result = false;

		std::for_each(colliders.cbegin(), colliders.cend(), [&result, &x1, &x2, &y1, &y2](const ColliderComponent* const pColl)
		{
			SDL_Rect rect = pColl->GetRect();
			if (SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
			{
				result = true;
				return;
			}
		});

		if (result)
		{
			pos.y = float(y1);
			m_Velocity.y = 0.f;
		}
	}

	//Update pos
	m_pParent->SetPosition(pos);
}

void anemoia::RigidBodyComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::RigidBodyComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::RigidBodyComponent::Render() const
{
	//Get renderer + collision box
	SDL_Renderer* const pRenderer = Locator::GetRenderer();
	SDL_Rect rect = m_pLinkedCollider->GetRect();

	const glm::vec2 midPoint(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f);

	//Draw important points
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, (int)midPoint.y, int(rect.x + rect.w * 0.5f), int(rect.y + rect.h));
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, (int)midPoint.y, int(rect.x), int(rect.y + rect.h * 0.5f));
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, (int)midPoint.y, int(rect.x + rect.w), int(rect.y + rect.h * 0.5f));
}

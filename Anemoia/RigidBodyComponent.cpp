#include "AnemoiaPCH.h"
#include "RigidBodyComponent.h"

#include "Locator.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"

anemoia::RigidBodyComponent::RigidBodyComponent(GameObject* const pParent, ColliderComponent* const pColl, float gravity)
	: BaseComponent(pParent, Transform()), m_pLinkedCollider{ pColl }, m_Gravity{gravity}
{
}

void anemoia::RigidBodyComponent::FixedUpdate(float timeStep)
{
	//We gonna modify pos
	glm::vec3 pos = GetParent()->GetPosition();

	//Apply gravity
	m_Velocity.y += m_Gravity * timeStep;

	//Apply velocity, test for collisions, update pos for correct collision rect
	pos += glm::vec3(m_Velocity, 0.f) * timeStep;
	m_pParent->SetPosition(pos);

	//Do collision checks
	CheckCollision();

	//Out of bounds?
	CheckOutOfBounds();
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
#if _DEBUG
	//Get renderer + collision box
	SDL_Renderer* const pRenderer = Locator::GetRenderer();
	SDL_Rect rect = m_pLinkedCollider->GetRect();

	const glm::vec2 midPoint(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f);

	//Draw important points
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, int(rect.y + rect.h * 0.9f), int(rect.x + rect.w * 0.5f), int(rect.y + rect.h)); //BOTTOM
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, (int)midPoint.y, int(rect.x), int(rect.y + rect.h * 0.5f)); //LEFT
	SDL_RenderDrawLine(pRenderer, (int)midPoint.x, (int)midPoint.y, int(rect.x + rect.w), int(rect.y + rect.h * 0.5f)); //RIGHT
#endif
}

void anemoia::RigidBodyComponent::Move(const glm::vec2& newPos)
{
	//Move
	glm::vec3 pos = GetParent()->GetPosition();
	pos.x = newPos.x;
	pos.y = newPos.y;
	m_pParent->SetPosition(pos);

	//Then check collision otherwise the checks will be "late"
	CheckCollision();
}

void anemoia::RigidBodyComponent::AddVelocity(const glm::vec2& value)
{
	m_Velocity += value;
}

void anemoia::RigidBodyComponent::SetVelocity(const glm::vec2& value)
{
	m_Velocity = value;
}

const glm::vec2& anemoia::RigidBodyComponent::GetVelocity() const
{
	return m_Velocity;
}

bool anemoia::RigidBodyComponent::IsTouchingFloor() const
{
	return m_IsTouchingFloor;
}

void anemoia::RigidBodyComponent::SetTouchingFloor(bool value)
{
	m_IsTouchingFloor = value;
}

bool anemoia::RigidBodyComponent::AddIgnoreTag(const std::string& tag)
{
	//This returns whether it was actually inserted or not
	return m_IgnoreTags.insert(tag).second;
}

bool anemoia::RigidBodyComponent::RemoveIgnoreTag(const std::string& tag)
{	
	//Returns whether something has been removed or not
	return m_IgnoreTags.erase(tag);
}

const std::unordered_set<std::string>& anemoia::RigidBodyComponent::GetIgnoreTags() const
{
	return m_IgnoreTags;
}

void anemoia::RigidBodyComponent::CheckCollision()
{
	//Used later down the line
	glm::vec3 pos = GetParent()->GetPosition();

	//Collision
	SDL_Rect rect = m_pLinkedCollider->GetRect();
	std::vector<ColliderComponent*> colliders = GetParent()->GetParentScene()->GetColliders();
	colliders.erase(std::find(colliders.cbegin(), colliders.cend(), m_pLinkedCollider)); //NO SELF COLLIDE

	//Remove any collider that has a tag contained within the "ignoretag" of this rigidbody
	{
		const std::vector<ColliderComponent*>::const_iterator cIt = std::remove_if(colliders.begin(), colliders.end(), [this](ColliderComponent* const pColl)
		{
			//Also any disabled game objects
			if (!pColl->GetParent()->IsEnabled()) { return true; }

			return std::any_of(m_IgnoreTags.cbegin(), m_IgnoreTags.cend(), [pColl](const std::string& tag)
			{
				return pColl->GetParent()->HasTag(tag);
			});
		});
		colliders.erase(cIt, colliders.cend());
	}

	//Remove any other collider which has ignore tags that match our tags
	{
		const std::vector<ColliderComponent*>::const_iterator cIt = std::remove_if(colliders.begin(), colliders.end(), [this](ColliderComponent* const pColl)
		{
			//Ignore tags only exist on rigid bodies
			RigidBodyComponent* const pOtherRigid = pColl->GetParent()->GetComponent<RigidBodyComponent>();
			if (pOtherRigid)
			{
				const std::unordered_set<std::string>& ignoreTags = pOtherRigid->GetIgnoreTags();
				return std::any_of(ignoreTags.cbegin(), ignoreTags.cend(), [this](const std::string& tag)
				{
					return m_pParent->HasTag(tag);
				});
			}

			return false;
		});
		colliders.erase(cIt, colliders.cend());
	}

	//BOTTOM COLLISION
	if (m_Velocity.y > 0.f)
	{
		//Line setup
		int x1 = int(rect.x + rect.w * 0.5f);
		int y1 = int(rect.y + rect.h * 0.9f);
		int x2 = x1;
		int y2 = int(rect.y + rect.h);
		int offset = (int)pos.y - y2;

		m_IsTouchingFloor = false;

		for (size_t i{}; i < colliders.size(); ++i)
		{
			SDL_Rect box = colliders[i]->GetRect();
			if (SDL_IntersectRectAndLine(&box, &x1, &y1, &x2, &y2))
			{
				//Notify
				m_IsTouchingFloor = true;
				colliders[i]->OnCollide(GetParent());
				GetParent()->OnCollide(colliders[i]->GetParent());

				//Adjust
				if (colliders[i]->AffectRigids())
				{
					pos.y = float(y1 + offset);
					m_Velocity.y = 1.f;
				}

				break;
			}
		}
	}
	else
	{
		m_IsTouchingFloor = false;
	}

	//LEFT COLLISION
	{
		//Update pos
		m_pParent->SetPosition(pos);
		rect = m_pLinkedCollider->GetRect();

		//Line setup
		int x1 = int(rect.x + rect.w * 0.5f);
		int y1 = int(rect.y + rect.h * 0.5f);
		int x2 = int(rect.x);
		int y2 = y1;
		const float offset = pos.x - float(x2);

		for (size_t i{}; i < colliders.size(); ++i)
		{
			if (!colliders[i]->IsImportant())
			{
				if (m_Velocity.y < 0.f)
				{
					continue;
				}
			}

			SDL_Rect box = colliders[i]->GetRect();
			if (SDL_IntersectRectAndLine(&box, &x1, &y1, &x2, &y2))
			{
				//Notify
				colliders[i]->OnCollide(GetParent());
				GetParent()->OnCollide(colliders[i]->GetParent());

				//Adjust
				if (colliders[i]->AffectRigids())
				{
					pos.x = x1 + offset;
					m_Velocity.x = 0.f;
				}

				break;
			}
		}
	}

	//RIGHT COLLISION
	{
		//Update pos
		m_pParent->SetPosition(pos);
		rect = m_pLinkedCollider->GetRect();

		//Line setup
		int x1 = int(rect.x + rect.w * 0.5f);
		int y1 = int(rect.y + rect.h * 0.5f);
		int x2 = int(rect.x + rect.w);
		int y2 = y1;
		const float offset = pos.x - (float)x2 + 1.f;

		for (size_t i{}; i < colliders.size(); ++i)
		{
			if (!colliders[i]->IsImportant())
			{
				if (m_Velocity.y < 0.f)
				{
					continue;
				}
			}

			SDL_Rect box = colliders[i]->GetRect();
			if (SDL_IntersectRectAndLine(&box, &x1, &y1, &x2, &y2))
			{
				//Notify
				colliders[i]->OnCollide(GetParent());
				GetParent()->OnCollide(colliders[i]->GetParent());

				//Adjust
				if (colliders[i]->AffectRigids())
				{
					pos.x = x1 + offset;
					m_Velocity.x = 0.f;
				}

				break;
			}
		}
	}

	//Update pos
	m_pParent->SetPosition(pos);
}

void anemoia::RigidBodyComponent::CheckOutOfBounds()
{
	//Get data
	int x, y;
	SDL_GetWindowSize(anemoia::Locator::GetWindow(), &x, &y);
	glm::vec3 pos = GetParent()->GetPosition();

	if (pos.y > float(y))
	{
		pos.y -= float(y);
		GetParent()->SetPosition(pos);
	}
}

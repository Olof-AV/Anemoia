#pragma once
#include "Transform.h"

namespace anemoia
{
	class BaseComponent;

	class GameObject final
	{
	public:
#pragma region Constructors
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
#pragma endregion Constructors

		//Game loop stuff
		void FixedUpdate(float timeStep);
		void Update(float elapsedSec);
		void Render() const;

		//Transform
		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& newPos);
		void SetPosition(float x, float y, float z);

#pragma region Components
		//Adding/removing comps
		void AddComponent(BaseComponent* const pComp);
		bool RemoveComponent(BaseComponent* const pComp);

		//Obtaining 1 component
		template<class T>
		T* GetComponent()
		{
			//Get type info requested
			const std::type_info& tInfo = typeid(T);
			for (BaseComponent* const pComp : m_Components)
			{
				//Loop over all components and the first one to match is sent back
				if (pComp && typeid(*pComp) == tInfo)
				{
					//When found, return the dynamically cast ver
					return dynamic_cast<T*>(pComp);
				}
			}

			return nullptr;
		}

		//Obtaining multiple components
		template<class T>
		bool GetComponents(std::vector<T*>& OutComps)
		{
			bool anyFound = false;

			//Get type info requested
			const std::type_info& tInfo = typeid(T);
			for (BaseComponent* const pComp : m_Components)
			{
				//Loop over all components and the first one to match is sent back
				if (pComp && typeid(*pComp) == tInfo)
				{
					//When found, return the dynamically cast ver
					OutComps.push_back(dynamic_cast<T*>(pComp));
					anyFound = true;
				}
			}

			//When finished, we can return whether there was a single find or nah
			return anyFound;
		}

#pragma endregion Components

	private:
		glm::vec3 m_Position;
		std::vector<BaseComponent*> m_Components;
	};
}
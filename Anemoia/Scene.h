#pragma once
#include "SceneManager.h"

namespace anemoia
{
	class GameObject;
	class ColliderComponent;

	class Scene
	{
	public:
#pragma region Constructors
		explicit Scene(const std::string& name);
		virtual ~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
#pragma endregion Constructors

		//GameObject stuff
		void AddChild(GameObject* const pObject);

		//Update loop stuff
		virtual void FixedUpdate(float timeStep);
		virtual void Update(float elapsedSec);
		virtual void LateUpdate(float elapsedSec);
		virtual void Render() const;

		//Scene stuff
		virtual void Initialise();
		virtual void OnSceneActivated();
		virtual void OnSceneDeactivated();

		//Getter
		const std::string& GetName() const;
		void SetName(const std::string& name);

		//Collision stuff
		void AddCollider(ColliderComponent* const pColl);
		void RemoveCollider(ColliderComponent* const pColl);
		const std::vector<ColliderComponent*> &GetColliders() const;

		//Tags
		GameObject* GetObjectWithTag(const std::string& tag) const;

	private:
		std::string m_Name;
		std::vector<GameObject*> m_Objects{};

		std::vector<ColliderComponent*> m_Collision{};

		//static unsigned int m_IdCounter;
	};
}
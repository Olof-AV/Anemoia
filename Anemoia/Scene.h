#pragma once
#include "SceneManager.h"

namespace anemoia
{
	class GameObject;

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
		virtual void Render() const;

		//Scene stuff
		virtual void Initialise();

		//Getter
		const std::string& GetName() const;

	private:
		std::string m_Name;
		std::vector<GameObject*> m_Objects{};

		//static unsigned int m_IdCounter;
	};
}
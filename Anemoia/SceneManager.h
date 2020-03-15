#pragma once
#include "Singleton.h"


namespace anemoia
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		//Update loop stuff
		void FixedUpdate(float timeStep);
		void Update(float elapsedSec);
		void Render();

		//Scene stuff
		void AddScene(Scene* const pScene);
		void InitialiseScenes() const;

	private:
		friend class Singleton<SceneManager>;

		SceneManager() = default;
		virtual ~SceneManager();
		std::vector<Scene*> m_Scenes;
	};
}
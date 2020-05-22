#pragma once
#include "Singleton.h"

#include <functional>

namespace anemoia
{
	class Scene;
	class GameObject;

	struct BoundFunc
	{
		std::function<void(void)> func;
		Scene* pScene = nullptr;
		GameObject* pParent = nullptr;
	};
	bool operator==(const anemoia::BoundFunc& a, const anemoia::BoundFunc& b);

	class AIManager final : public Singleton<AIManager>
	{
	public:
		void RegisterFunction(const BoundFunc &func);
		void UnRegisterFunction(const BoundFunc& func);

		void Run();
		void Stop();

	private:
		friend class Singleton<AIManager>;

		AIManager();
		virtual ~AIManager();

		//Functionality
		bool m_IsRunning = false;
		std::vector<BoundFunc> m_Functions;
	};
}


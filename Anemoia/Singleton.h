#pragma once

namespace anemoia
{
	template <typename T>
	class Singleton
	{
	public:
		static T* GetInstance()
		{
			if (m_pInstance)
			{
				return m_pInstance;
			}
			m_pInstance = new T;
			return m_pInstance;
		}

		static void DestroyInstance()
		{
			if (m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}

		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	private:
		inline static T* m_pInstance = nullptr;
	};
}
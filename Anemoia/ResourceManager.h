#pragma once
#include "Singleton.h"
#include <unordered_map>

namespace anemoia
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);

	private:
		friend class Singleton<ResourceManager>;

		//Constructor/destructor
		ResourceManager() = default;
		virtual ~ResourceManager();

		//Data members
		std::string m_DataPath;
		std::unordered_map<std::string, class Resource*> m_SavedResources;
	};
}
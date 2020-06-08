#include "AnemoiaPCH.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Sound.h"

#include <algorithm>

#include "Locator.h"

void anemoia::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	//More image stuff
	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	//Fonts
	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}

	//Initialise audio + open audio device
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) == 0)
	{
		throw std::runtime_error(std::string("Failed to load support for audio: ") + SDL_GetError());
	}
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		throw std::runtime_error(std::string("Failed to open audio: ") + Mix_GetError());
	}
	if (Mix_AllocateChannels(1024) == 0)
	{
		throw std::runtime_error(std::string("Failed to allocate channels: ") + Mix_GetError());
	}
}

anemoia::Texture2D* anemoia::ResourceManager::LoadTexture(const std::string& file)
{
	//Look for filename
	std::unordered_map<const std::string, Resource*>::const_iterator cIt = m_SavedResources.find(file);

	//If not found, create
	if (cIt == m_SavedResources.cend())
	{
		const auto fullPath = m_DataPath + file;
		SDL_Texture* const pTexture = IMG_LoadTexture(Locator::GetRenderer(), fullPath.c_str());
		if (pTexture == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		}

		//Get dimensions
		int w, h;
		SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);

		//Create and insert into saved resources
		Texture2D* const pTexture2D = new Texture2D(pTexture, glm::vec2(float(w), float(h)));
		m_SavedResources.insert(std::make_pair(file, pTexture2D));
		return pTexture2D;
	}

	//Return found font
	return dynamic_cast<Texture2D*>((*cIt).second);
}

anemoia::Font* anemoia::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	//Look for filename
	std::unordered_map<const std::string, Resource*>::const_iterator cIt = m_SavedResources.find(file);

	//If not found, create
	if (cIt == m_SavedResources.cend())
	{
		Font* const pFont = new Font(m_DataPath + file, size);
		m_SavedResources.insert(std::make_pair(file, pFont));
		return pFont;
	}

	//Return found font
	return dynamic_cast<Font*>((*cIt).second);
}

anemoia::Sound* anemoia::ResourceManager::LoadSound(const std::string& file)
{
	//Look for filename
	std::unordered_map<const std::string, Resource*>::const_iterator cIt = m_SavedResources.find(file);

	//If not found, create
	if (cIt == m_SavedResources.cend())
	{
		Mix_Chunk* const pChunk = Mix_LoadWAV((m_DataPath + file).c_str());
		Sound* const pSound = new Sound(pChunk);
		m_SavedResources.insert(std::make_pair(file, pSound));
		return pSound;
	}

	//Return found font
	return dynamic_cast<Sound*>((*cIt).second);
}

const std::string& anemoia::ResourceManager::GetDataPath() const
{
	return m_DataPath;
}

anemoia::ResourceManager::~ResourceManager()
{
	//Delete all distributed resources
	std::for_each(m_SavedResources.cbegin(), m_SavedResources.cend(), [](const std::pair<std::string, Resource*> &pair)
	{
		delete pair.second;
	});

	//Remember to close down libraries
	IMG_Quit();
	TTF_Quit();

	Mix_CloseAudio();
	Mix_Quit();
}
#include "AnemoiaPCH.h"
#include "Sound.h"

#include <SDL_mixer.h>

int anemoia::Sound::IdCounter = 0;

anemoia::Sound::Sound(Mix_Chunk* const pChunk)
	: Resource{}, m_pChunk{ pChunk }, m_Channel{ IdCounter++ }
{

}

anemoia::Sound::~Sound()
{
	//Don't forget to free the chunk
	if (m_pChunk)
	{
		Mix_FreeChunk(m_pChunk);
	}
}

void anemoia::Sound::Play(int loopAmount) const
{
	Mix_PlayChannel(m_Channel, m_pChunk, loopAmount);
}

void anemoia::Sound::Pause() const
{
	Mix_Pause(m_Channel);
}

void anemoia::Sound::Stop() const
{
	Mix_HaltChannel(m_Channel);
}

bool anemoia::Sound::IsPlaying() const
{
	return Mix_Playing(m_Channel);
}

bool anemoia::Sound::IsPaused() const
{
	return Mix_Paused(m_Channel);
}

void anemoia::Sound::FadeIn(int loopAmount, int milliseconds)
{
	Mix_FadeInChannel(m_Channel, m_pChunk, loopAmount, milliseconds);
}

void anemoia::Sound::FadeOut(int milliseconds)
{
	Mix_FadeOutChannel(m_Channel, milliseconds);
}

void anemoia::Sound::SetVolume(float volume) const
{
	Mix_VolumeChunk(m_pChunk, int(volume * 128.f));
}

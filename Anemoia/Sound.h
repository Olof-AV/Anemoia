#pragma once
#include "Resource.h"

struct Mix_Chunk;

namespace anemoia
{
	class Sound final : public Resource
	{
	public:
#pragma region Constructors
		Sound(Mix_Chunk* const pChunk);
		virtual ~Sound();

		Sound(const Sound&) = delete;
		Sound(Sound&&) = delete;
		Sound& operator= (const Sound&) = delete;
		Sound& operator= (const Sound&&) = delete;
#pragma endregion Constructors

		void Play(int loopAmount) const;
		void Pause() const;
		void Stop() const;

		bool IsPlaying() const;
		bool IsPaused() const;

		void FadeIn(int loopAmount, int milliseconds);
		void FadeOut(int milliseconds);

		void SetVolume(float volume) const;

	private:
		Mix_Chunk* m_pChunk;
		int m_Channel;

		static int IdCounter;
	};
}


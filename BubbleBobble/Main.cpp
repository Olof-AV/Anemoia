#include "pch.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "BubbleBobbleGame.h"

#include "SDL_mixer.h"

int main(int, char* [])
{
	BubbleBobbleGame pEngine;
	pEngine.Run();

	return 0;
}
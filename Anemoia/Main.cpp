#include "AnemoiaPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "Anemoia.h"

int main(int, char*[]) {
	Anemoia::Engine engine;
	engine.Run();
    return 0;
}
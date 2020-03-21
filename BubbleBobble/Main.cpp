#include "pch.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "BubbleBobbleGame.h"

//#define CATCH_CONFIG_RUNNER    // Designates this as implementation file
#define CATCH_CONFIG_MAIN      // Designates this as implementation file and defines main()
#include "catch.hpp"

/*int main(int argc, char* argv[])
{
	const int result = Catch::Session().run(argc, argv);
	return result;
}*/

TEST_CASE("Game")
{
	BubbleBobbleGame game;
	CHECK_NOTHROW(game.Run());
}
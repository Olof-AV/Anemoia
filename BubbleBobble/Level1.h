#pragma once
#include "BaseGameScene.h"

namespace anemoia
{
	class TextComponent;
	class TextureComponent;
	class FPSComponent;
	class Sound;
}

class Level1 final : public BaseGameScene
{
public:
#pragma region Constructors
	Level1();
	virtual ~Level1();

	Level1(const Level1& other) = delete;
	Level1(Level1&& other) = delete;
	Level1& operator=(const Level1& other) = delete;
	Level1& operator=(Level1&& other) = delete;
#pragma endregion Constructors

	//Overrides
	virtual void FixedUpdate(float timeStep);
	virtual void Update(float elapsedSec);
	virtual void LateUpdate(float elapsedSec);
	virtual void Render() const;

	//Scene stuff
	virtual void Initialise();

private:

};
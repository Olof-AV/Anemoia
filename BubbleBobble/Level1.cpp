#include "pch.h"
#include "Level1.h"

Level1::Level1()
	: BaseGameScene("Level1")
{
}

Level1::~Level1()
{
}

void Level1::FixedUpdate(float timeStep)
{
	//Call root
	BaseGameScene::FixedUpdate(timeStep);
}

void Level1::Update(float elapsedSec)
{
	//Call root
	BaseGameScene::Update(elapsedSec);
}

void Level1::LateUpdate(float elapsedSec)
{
	//Call root
	BaseGameScene::LateUpdate(elapsedSec);
}

void Level1::Render() const
{
	//Call root
	BaseGameScene::Render();
}

void Level1::Initialise()
{
	//Call root
	BaseGameScene::Initialise();
}

void Level1::OnSceneActivated()
{
	//Call root
	BaseGameScene::OnSceneActivated();
}

void Level1::OnSceneDeactivated()
{
	//Call root
	BaseGameScene::OnSceneDeactivated();
}

#pragma once
#include "BaseComponent.h"

class Texture2D;

class TextureComponent final : public BaseComponent
{
public:
#pragma region Constructors
	TextureComponent(GameObject* const pParent, const Transform& transform,
		Texture2D* const pTexture);

	virtual ~TextureComponent() = default;

	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) = delete;
#pragma endregion Constructors

	//Overrides
	virtual void FixedUpdate(float timeStep) override;
	virtual void Update(float elapsedSec) override;
	virtual void Render() const override;

private:
	Texture2D* m_pTexture;
};


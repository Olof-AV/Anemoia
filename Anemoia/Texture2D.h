#pragma once
#include "Resource.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

struct SDL_Texture;

	/**
	* Simple RAII wrapper for an SDL_Texture
	*/
class Texture2D final : public Resource
{
public:
#pragma region Constructors
	Texture2D(SDL_Texture* const pTexture, const glm::vec2 dimensions);
	virtual ~Texture2D();

	Texture2D(const Texture2D &) = delete;
	Texture2D(Texture2D &&) = delete;
	Texture2D & operator= (const Texture2D &) = delete;
	Texture2D & operator= (const Texture2D &&) = delete;
#pragma endregion Constructors

	//Texture
	SDL_Texture* GetSDLTexture() const;
	const glm::vec2 &GetDimensions() const;

private:
	SDL_Texture* m_pTexture;
	glm::vec2 m_Dimensions;
};
#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
class Texture2D;

#include <SDL_render.h>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

	/**
	* Simple RAII wrapper for the SDL renderer
	*/
class Renderer final : public Singleton<Renderer>
{
public:
	void Init(SDL_Window* const pWindow);
	void Render() const;

	//Texture render stuff
	void RenderTexture(Texture2D* const pTexture, float x, float y) const;
	void RenderTexture(Texture2D* const pTexture, float x, float y, float width, float height) const;
	void RenderTexture(Texture2D* const pTexture, float x, float y, float width, float height, float angle, const glm::vec2 &pivotCenter, SDL_RendererFlip flip) const;

	//Replaced by locator
	//SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

private:
	friend class Singleton<Renderer>;

	virtual ~Renderer();
	void Destroy();
	SDL_Renderer* m_Renderer{};
};

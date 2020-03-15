#include "AnemoiaPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

#include "Locator.h"

void Renderer::Init(SDL_Window * const pWindow)
{
	//Create accelerated renderer, in VSYNC mode
	m_Renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//Setup locator
	Locator::SetSDL_Renderer(m_Renderer);
}

void Renderer::Render() const
{
	//Clear renderer
	SDL_RenderClear(m_Renderer);

	//Render everything in scenes
	SceneManager::GetInstance()->Render();
	
	//Present
	SDL_RenderPresent(m_Renderer);
}

void Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::RenderTexture(Texture2D* const pTexture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_Renderer, pTexture->GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D* const pTexture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(m_Renderer, pTexture->GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(Texture2D* const pTexture, float x, float y, float width, float height, float angle, const glm::vec2 &pivotCenter, SDL_RendererFlip flip) const
{
	//Setup
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	SDL_Point point;
	point.x = (int)pivotCenter.x;
	point.y = (int)pivotCenter.y;

	//Render
	SDL_RenderCopyEx(m_Renderer, pTexture->GetSDLTexture(), nullptr, &dst, angle, &point, flip);
}

#include "AnemoiaPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

#include "Locator.h"

void anemoia::Renderer::Init(SDL_Window * const pWindow)
{
	//Create accelerated renderer, in VSYNC mode
	m_pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//Setup locator
	Locator::SetSDL_Renderer(m_pRenderer);

	//Set global render draw blendmode
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
}

void anemoia::Renderer::Render() const
{
	//Clear renderer
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	//Render everything in scenes
	SceneManager::GetInstance()->Render();
	
	//Present
	SDL_RenderPresent(m_pRenderer);
}

void anemoia::Renderer::Destroy()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

anemoia::Renderer::~Renderer()
{
	Destroy();
}

void anemoia::Renderer::RenderTexture(anemoia::Texture2D* const pTexture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_pRenderer, pTexture->GetSDLTexture(), nullptr, &dst);
}

void anemoia::Renderer::RenderTexture(anemoia::Texture2D* const pTexture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(m_pRenderer, pTexture->GetSDLTexture(), nullptr, &dst);
}

#pragma warning(suppress:26812)
void anemoia::Renderer::RenderTexture(anemoia::Texture2D* const pTexture,
	float x, float y, float width, float height,
	float angle, const glm::vec2 &pivotCenter, SDL_RendererFlip flip,
	const glm::vec2& clipOrigin, const glm::vec2& clipSize) const
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

	if (clipSize.x < 0.f || clipSize.y < 0.f)
	{
		//Render
		SDL_RenderCopyEx(m_pRenderer, pTexture->GetSDLTexture(), nullptr, &dst, angle, &point, flip);
	}
	else
	{
		//Also set up clip
		SDL_Rect src;
		src.x = static_cast<int>(clipOrigin.x);
		src.y = static_cast<int>(clipOrigin.y);
		src.w = static_cast<int>(clipSize.x);
		src.h = static_cast<int>(clipSize.y);

		//Render
		SDL_RenderCopyEx(m_pRenderer, pTexture->GetSDLTexture(), &src, &dst, angle, &point, flip);
	}
}

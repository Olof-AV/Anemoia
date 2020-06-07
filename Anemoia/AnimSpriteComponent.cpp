#include "AnemoiaPCH.h"
#include "AnimSpriteComponent.h"

#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

#include "Locator.h"

#include <regex>
#include <fstream>
#include "ResourceManager.h"

anemoia::AnimSpriteComponent::AnimSpriteComponent(GameObject* const pParent, const Transform& transform,
	const std::string& animPath, const std::string& defaultAnim, const glm::vec4& colour)
	: BaseComponent(pParent, transform), m_ColourMod{ colour }
{
	//Reads: spritesheet path, frame count, nr of cols, nr of rows, frames per sec, animation name
	const std::regex myRegex{ R"(^\"(.+)\",(\d+),(\d+),(\d+),(\d+(?:[.]\d+)?),\"(.+)\"$)" };

	//Store matches in here
	std::smatch matches;

	std::ifstream fileStream(anemoia::ResourceManager::GetInstance()->GetDataPath() + animPath);
	while (fileStream)
	{
		std::string line;
		std::getline(fileStream, line);

		//No empty lines
		if (!line.empty())
		{
			//Does it match the regex?
			if (std::regex_search(line, matches, myRegex))
			{
				//Should have exactly 6 + 1 entire match
				if (matches.size() == 7)
				{
					AnimSprite toCreate{};
					toCreate.pTexture = anemoia::ResourceManager::GetInstance()->LoadTexture(matches[1]);
					toCreate.frameCount = std::stoi(matches[2]);
					toCreate.nrCols = std::stoi(matches[3]);
					toCreate.nrRows = std::stoi(matches[4]);
					toCreate.framesPerSec = std::stof(matches[5]);
					toCreate.totalTime = 0.f;

					m_Sprites[matches[6]] = toCreate;
				}
			}
		}
	}

	SetAnim(defaultAnim);
}

void anemoia::AnimSpriteComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void anemoia::AnimSpriteComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
	m_Sprites[m_ActiveSprite].totalTime += elapsedSec;
}

void anemoia::AnimSpriteComponent::LateUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void anemoia::AnimSpriteComponent::Render() const
{
	anemoia::AnimSprite currentSprite = m_Sprites.at(m_ActiveSprite);
	anemoia::Texture2D* const pTex = currentSprite.pTexture;

	//Calculate params
	glm::vec2 size = pTex->GetDimensions() * m_Transform.GetScale();
	size.x /= currentSprite.nrCols;
	size.y /= currentSprite.nrRows;

	const glm::vec2 pivotOffset = (m_Transform.GetPivot() * size);
	const glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;

	//Obtain global blend mode and set that as texture blend mode
	SDL_BlendMode blendMode;
	SDL_GetRenderDrawBlendMode(Locator::GetRenderer(), &blendMode);
	SDL_SetTextureBlendMode(pTex->GetSDLTexture(), blendMode);

	//Set colour according to mod
	SDL_SetTextureColorMod(pTex->GetSDLTexture(), (Uint8)m_ColourMod.r, (Uint8)m_ColourMod.g, (Uint8)m_ColourMod.b);
	SDL_SetTextureAlphaMod(pTex->GetSDLTexture(), (Uint8)m_ColourMod.a);

	//Get current frame
	const int currentFrame = int(std::fmodf(currentSprite.totalTime * currentSprite.framesPerSec, float(currentSprite.frameCount)));
	const int col = currentFrame % currentSprite.nrCols;
	const int row = currentFrame / currentSprite.nrCols;
	const glm::vec2 clipSize{ float(pTex->GetDimensions().x / currentSprite.nrCols), float(pTex->GetDimensions().y / currentSprite.nrRows) };
	const glm::vec2 clipOrigin{ float(clipSize.x *col), float(clipSize.y * row) };

	//Render texture
	Renderer::GetInstance()->RenderTexture(pTex, finalPos.x, finalPos.y, size.x, size.y,
		m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip(), clipOrigin, clipSize);
}

const std::string &anemoia::AnimSpriteComponent::GetAnim() const
{
	return m_ActiveSprite;
}

void anemoia::AnimSpriteComponent::SetAnim(const std::string& name)
{
	std::unordered_map<std::string,AnimSprite>::const_iterator cIt = m_Sprites.find(name);
	if (cIt == m_Sprites.cend())
	{
		throw std::runtime_error(name + " is not a valid animation.");
	}

	m_ActiveSprite = name;
}

const glm::vec4& anemoia::AnimSpriteComponent::GetColourMod() const
{
	return m_ColourMod;
}

void anemoia::AnimSpriteComponent::SetColourMod(const glm::vec4& colour)
{
	m_ColourMod = colour;
}

float anemoia::AnimSpriteComponent::GetAlpha() const
{
	return m_ColourMod.a;
}

void anemoia::AnimSpriteComponent::SetAlpha(float value)
{
	m_ColourMod.a = value;
}

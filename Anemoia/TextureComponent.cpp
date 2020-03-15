#include "AnemoiaPCH.h"
#include "TextureComponent.h"

#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

TextureComponent::TextureComponent(GameObject* const pParent, const Transform& transform,
	Texture2D* const pTexture)
	: BaseComponent(pParent, transform), m_pTexture(pTexture)
{
}

void TextureComponent::FixedUpdate(float timeStep)
{
	UNREFERENCED_PARAMETER(timeStep);
}

void TextureComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void TextureComponent::Render() const
{
	//Calculate params
	const glm::vec2 size = m_pTexture->GetDimensions() * m_Transform.GetScale();
	const glm::vec2 pivotOffset = (m_Transform.GetPivot() * size);
	const glm::vec2 finalPos = glm::vec2(m_Transform.GetPosition() + GetParent()->GetPosition()) - pivotOffset;

	//Render texture
	Renderer::GetInstance()->RenderTexture(m_pTexture, finalPos.x, finalPos.y, size.x, size.y, m_Transform.GetAngle(), pivotOffset, m_Transform.GetFlip());
}

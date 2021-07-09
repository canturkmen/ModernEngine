#include "mnpch.h"
#include "SubTexture2D.h"

namespace ModernEngine {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& textureCoords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (textureCoords.x * cellSize.x) / texture->GetWidth(), (textureCoords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((textureCoords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((textureCoords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return std::make_shared<SubTexture2D>(texture, min, max);
	}
}

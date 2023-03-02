#pragma once

#include <string>
#include "ModernEngine/Core/Base.h"

namespace ModernEngine {

	enum class ImageFormat
	{
		None = 0,
		R8,	
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::string& GetFilePath() const = 0;

		virtual void SetData(void *data, uint32_t size) = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};
	
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::string& filepath);
	};
}


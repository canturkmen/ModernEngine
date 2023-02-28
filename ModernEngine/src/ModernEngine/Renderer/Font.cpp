#include "mnpch.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "GlyphGeometry.h"

namespace ModernEngine {

	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};

	Font::Font(const std::filesystem::path& filePath)
		: m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		if (ft) {
			std::string sFilePath = filePath.string();
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, sFilePath.c_str());

			if (!font) 
			{ 
				MN_CORE_ERROR("Failed to load font {}", sFilePath);
				return;
			}

			struct CharsetRange
			{
				uint32_t Begin, End;
			};

			static const CharsetRange charsetRanges[] =
			{
				{0x0020, 0x00FF}
			};
			
			msdf_atlas::Charset charset;
			for (CharsetRange range : charsetRanges)
			{
				for (uint32_t c = range.Begin; c <= range.End; c++)
					charset.add(c);
			}

			double fontScale = 1.0f;
			m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
			int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
			MN_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());
#if 0
			msdfgen::Shape shape;
			if (msdfgen::loadGlyph(shape, font, 'C')) {
				shape.normalize();
				//                      max. angle
				msdfgen::edgeColoringSimple(shape, 3.0);
				//           image width, height
				msdfgen::Bitmap<float, 3> msdf(32, 32);
				//                     range, scale, translation
				msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
				msdfgen::savePng(msdf, "output.png");
			}
#endif
			
			msdfgen::destroyFont(font);
			msdfgen::deinitializeFreetype(ft);
		}
	}

	Font::~Font()
	{
		delete m_Data;
	}
}


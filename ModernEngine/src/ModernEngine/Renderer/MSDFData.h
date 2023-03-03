#pragma once

#include <vector>

#undef INFINITE
#include "msdf-atlas-gen.h"

namespace ModernEngine {

	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};

}
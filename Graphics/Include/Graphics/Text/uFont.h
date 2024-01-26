#ifndef UFONT_H
#define UFONT_H

#include "Graphics/Text/uGlyph.h"

#include "ft2build.h"
#include FT_FREETYPE_H  

#include <map>
#include <vector>
#include <string>

class uFont {
    static FT_Library library;
    FT_Face face;

    std::map<int, std::map<uint32_t, uGlyph>> glyphs;

public:

    uFont(const char* filePath);

    std::vector<uint32_t> GetCodepointsFromString(std::string string);

    uGlyph GetGlyphForCodepoint(uint32_t codePoint, float fontSize);
};

#endif // UFONT_H

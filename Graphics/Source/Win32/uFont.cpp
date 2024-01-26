#include "Graphics/Text/uFont.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>

FT_Library uFont::library;

uFont::uFont(const char* filePath) {
    if (library == nullptr) {
        if (FT_Init_FreeType(&library)) { 
            printf("critical: freetype failed to initialize!\n");
        }
    }

    if (FT_New_Face(library, filePath, 0, &face)) {
        printf("critical: freetype failed to create font face!\n");
    }
}

std::vector<uint32_t> uFont::GetCodepointsFromString(std::string text) {
    // chatGPT UTF-8 conversion
    std::vector<uint32_t> codepoints;
    for (auto it = text.begin(); it != text.end();) {
        uint32_t cp = 0;
        unsigned char lead = *it;
        if (lead < 0x80) {
            cp = *it++;
        } else if ((lead >> 5) == 0x6) {
            cp = (*it++ & 0x1F) << 6;
            cp |= (*it++ & 0x3F);
        } else if ((lead >> 4) == 0xE) {
            cp = (*it++ & 0x0F) << 12;
            cp |= (*it++ & 0x3F) << 6;
            cp |= (*it++ & 0x3F);
        } else if ((lead >> 3) == 0x1E) {
            cp = (*it++ & 0x07) << 18;
            cp |= (*it++ & 0x3F) << 12;
            cp |= (*it++ & 0x3F) << 6;
            cp |= (*it++ & 0x3F);
        } else {
            // Invalid UTF-8 sequence, skip it
            ++it;
            continue;
        }
        codepoints.push_back(cp);
    }

    return codepoints;
}


uGlyph uFont::GetGlyphForCodepoint(uint32_t codePoint, float fontSize) {
    if (glyphs.count((int)fontSize) > 0) {
        // glyphs contains this font size
        if (glyphs[(int) fontSize].count(codePoint) > 0) {
            // this codepoint exists for the specified font size
            return glyphs[(int) fontSize][codePoint];
        }
    }

    // the requested glyph & size doesn't exist
    //printf("new glyph %x %d\n", codePoint, (int)fontSize);


    uGlyph glyph;
    glyph.codepoint = codePoint;


    FT_Set_Pixel_Sizes(face, 0, (int)fontSize);
    FT_Render_Mode(FT_RENDER_MODE_LCD );
    if (FT_Load_Char(face, codePoint, FT_LOAD_RENDER)) {
        printf("ERROR::FREETYTPE: Failed to load Glyph\n");
        return glyph;
    }

    glGenTextures(1, &glyph.texture);
    glBindTexture(GL_TEXTURE_2D, glyph.texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_ALPHA,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_ALPHA,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    printf("%c size is %u %u\n", codePoint, face->glyph->bitmap.width, face->glyph->bitmap.rows );

     // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glyph.size = { (float) face->glyph->bitmap.width, (float) face->glyph->bitmap.rows };
    glyph.bearing = { (float) face->glyph->bitmap_left, (float) face->glyph->bitmap_top };
    glyph.advance = face->glyph->advance.x >> 6; // advance is in 1/64th px

    //printf("%d %f advance\n", glyph.advance, (float)glyph.advance);

    glyphs[(int) fontSize][codePoint] = glyph;

    return glyph;
}
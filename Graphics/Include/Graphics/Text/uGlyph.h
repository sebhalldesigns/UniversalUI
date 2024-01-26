#ifndef UGLYPH_H
#define UGLYPH_H

#include "Graphics/General/uPoint.h"
#include "Graphics/General/uSize.h"

#include <cstdint>

struct uGlyph {
    uint32_t codepoint;
    unsigned int texture;
    uPoint bearing;
    uSize size;
    unsigned int advance;
};


#endif // UGLYPH_H
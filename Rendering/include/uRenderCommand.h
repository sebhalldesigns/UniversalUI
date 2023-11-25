#ifndef URENDERCOMMAND_H
#define URENDERCOMMAND_H

#include <cstdint>

// internal class for building abstracted drawing commands
// for rendering at a later date
const uint8_t MAX_PARAMETERS = 4;

enum uRenderCommandType {
    BEGIN_POINTS,
    END_POINTS,

    BEGIN_LINES,
    END_LINES,

    BEGIN_LINE_STRIP,
    END_LINE_STRIP,

    BEGIN_LINE_LOOP,
    END_LINE_LOOP,

    BEGIN_TRIANGLES,
    END_TRIANGLES,

    BEGIN_TRIANGLE_STRIP,
    END_TRIANGLE_STRIP,

    BEGIN_TRIANGLE_FAN,
    END_TRIANGLE_FAN,

    BEGIN_QUADS,
    END_QUADS,

    BEGIN_QUADS_STRIP,
    END_QUADS_STRIP,

    BEGIN_POLYGON,
    END_POLYGON,

    BEGIN_TEXTURE,
    END_TEXTURE,

    VERTEX_2D,
    TEX_VERTEX_2D,

    COLOR_4F,

    POINT_SIZE,
    LINE_WIDTH


};

class uRenderCommand {
public:
    uRenderCommandType commandType;
    double parameters[MAX_PARAMETERS];
};


#endif // URENDERCOMMAND_H
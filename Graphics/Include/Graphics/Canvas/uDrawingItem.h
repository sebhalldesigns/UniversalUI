#ifndef UDRAWINGITEM_H
#define UDRAWINGITEM_H

#include "Graphics/Command/uDrawingCommand.h"
#include "Graphics/General/uPoint.h"
#include "Graphics/General/uColor.h"
#include "Graphics/General/uSize.h"
#include "Graphics/Text/uFont.h"


#include <vector>
#include <cstdio>

class uDrawingItem {

    friend class uCanvas;
    friend class uApplication;



protected:
    std::vector<uDrawingCommand> drawingCommands;
    virtual void CalculateCommands() { }
    
};

namespace Draw {
    class Line : public uDrawingItem {
        public:
            uPoint start;
            uPoint end;

            uColor color;
            float thickness;
        protected:
            void CalculateCommands() override {
                drawingCommands.push_back({ uDrawingCommandType::BEGIN_LINES });
                drawingCommands.push_back({ uDrawingCommandType::COLOR_4F, color.r, color.g, color.b, color.a });
                drawingCommands.push_back({ uDrawingCommandType::LINE_WIDTH, thickness });
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, start.x, start.y });
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, end.x, end.y });
                drawingCommands.push_back({ uDrawingCommandType::END_LINES });
            }
    };

    class RoundedRect : public uDrawingItem {
        public:
            uPoint origin;
            uSize size;

            uColor fillColor;
            float cornerRadius;
        protected:
            void CalculateCommands() override {
                drawingCommands.push_back({ uDrawingCommandType::COLOR_4F, fillColor.r, fillColor.g, fillColor.b, fillColor.a });
                drawingCommands.push_back({ uDrawingCommandType::BEGIN_POLYGON });

                // start with top left after radius
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + cornerRadius, origin.y });
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + size.width - cornerRadius, origin.y });
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + size.width, origin.y + cornerRadius });
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + size.width, origin.y + size.height - cornerRadius});
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + size.width - cornerRadius, origin.y + size.height});
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x + cornerRadius, origin.y + size.height});
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x, origin.y + size.height - cornerRadius});
                drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, origin.x, origin.y + cornerRadius});

                drawingCommands.push_back({ uDrawingCommandType::END_POLYGON });
            }
    };

    enum LabelDisplayMode {
        CLIP_TEXT,
        CLIP_WITH_ELLIPSIS,
        FIXED_HEIGHT_EXPAND_WIDTH,
        FIXED_WIDTH_EXPAND_HEIGHT,  
    };

    class Label : public uDrawingItem {
        public:
            uPoint origin;

            LabelDisplayMode displayMode;

            bool canBeWider = false;
            bool canBeTaller = false;
            uSize size;

            uFont* font;
            float fontSize = 13.0f;
            uColor fontColor;

            std::string text;

        protected:
            void CalculateCommands() override {
                if (font == nullptr) {
                    printf("error: no font provided!\n");
                    return;
                }

                
                std::vector<uint32_t> codePoints = font->GetCodepointsFromString(text);
                std::vector<uGlyph> glyphs;

                for (uint32_t codePoint : codePoints) {
                    glyphs.push_back(font->GetGlyphForCodepoint(codePoint, fontSize));
                }

                uPoint workingPoint = origin;

                for (uGlyph glyph : glyphs) {

                    float xpos = workingPoint.x + glyph.bearing.x;
                    float ypos = workingPoint.y + (glyph.size.height - glyph.bearing.y);

                    float w = glyph.size.width;
                    float h = glyph.size.height;
                    
                    drawingCommands.push_back({ uDrawingCommandType::BEGIN_TEXTURE,  0.0f, 0.0f, 0.0f, 0.0f, glyph.texture });
                    drawingCommands.push_back({ uDrawingCommandType::BEGIN_QUADS });
                    drawingCommands.push_back({ uDrawingCommandType::COLOR_4F, fontColor.r, fontColor.g, fontColor.b, fontColor.a });
                    drawingCommands.push_back({ uDrawingCommandType::TEX_VERTEX_2D, 0.0f, 1.0f });
                    drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, xpos, ypos});
                    drawingCommands.push_back({ uDrawingCommandType::TEX_VERTEX_2D, 1.0f, 1.0f });
                    drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, xpos + w, ypos});
                    drawingCommands.push_back({ uDrawingCommandType::TEX_VERTEX_2D, 1.0f, 0.0f });
                    drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, xpos + w, ypos - h});
                    drawingCommands.push_back({ uDrawingCommandType::TEX_VERTEX_2D, 0.0f, 0.0f });
                    drawingCommands.push_back({ uDrawingCommandType::VERTEX_2D, xpos, ypos - h});
                    drawingCommands.push_back({ uDrawingCommandType::END_QUADS });
                    drawingCommands.push_back({ uDrawingCommandType::END_TEXTURE });

                    if (workingPoint.x + (float)glyph.advance > origin.x + size.width) {
                        workingPoint.x = origin.x;
                        workingPoint.y += fontSize;
                    } else {
                        workingPoint.x += (float)glyph.advance;
                    }
                    
                    //printf("%f advance\n", (float)glyph.advance);
                }



            }
    };
}

#endif // UDRAWINGITEM_H


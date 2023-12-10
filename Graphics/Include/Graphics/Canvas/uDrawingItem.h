#ifndef UDRAWINGITEM_H
#define UDRAWINGITEM_H

#include "Graphics/Command/uDrawingCommand.h"
#include "Graphics/General/uPoint.h"
#include "Graphics/General/uColor.h"
#include "Graphics/General/uSize.h"


#include <vector>

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
}

#endif // UDRAWINGITEM_H


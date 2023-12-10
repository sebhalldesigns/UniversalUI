#ifndef UDRAWINGITEM_H
#define UDRAWINGITEM_H

#include "Graphics/Command/uDrawingCommand.h"
#include "Graphics/General/uPoint.h"
#include "Graphics/General/uColor.h"


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
}

#endif // UDRAWINGITEM_H


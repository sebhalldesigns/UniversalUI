#ifndef UCANVAS_H
#define UCANVAS_H

#include "Graphics/General/uColor.h"
#include "Graphics/General/uSize.h"
#include "Graphics/Command/uDrawingCommand.h"

#include "uDrawingItem.h"

#include <vector>

class uCanvas {
    friend class uRenderSurface;
    friend class uApplication;


    std::vector<uDrawingCommand> commands;

public:
    
    uColor backgroundColor;
    bool clipsToBounds;

    void AddItem(uDrawingItem& item) {
        item.CalculateCommands();
        commands.insert(commands.end(), item.drawingCommands.begin(), item.drawingCommands.end());
    }

    float x;
    float y;
    float width;
    float height;

};


#endif // UCANVAS_H
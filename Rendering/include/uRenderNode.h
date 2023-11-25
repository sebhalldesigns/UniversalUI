#ifndef URENDERTARGET_H
#define URENDERTARGET_H

#include <vector>

#include "uCanvas.h"
#include "uRenderCommand.h"


// internal class for managing view rendering stuff
class uRenderNode {


public: 

    std::vector<uRenderCommand> renderCommands;

    void UpdateRenderCommands(uCanvas& canvas);

};

#endif // URENDERTARGET_H
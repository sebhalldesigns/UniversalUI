#include "Graphics/Surface/uRenderSurface.h"

void RenderCommands(uCanvas* canvas, std::vector<uDrawingCommand>& commands);

uRenderSurface* uRenderSurface::InitForWindow(uWindowHandle windowHandle, float width, float height) {

    uRenderSurface* surface = new uRenderSurface;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24, // bit depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16, // depth buffer
        0, // stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC hdc = GetDC(windowHandle);
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    surface->resources.windowHandle = windowHandle;
    surface->resources.glRenderContextHandle = wglCreateContext(hdc);
    
    wglMakeCurrent(hdc, surface->resources.glRenderContextHandle);
    glViewport(0, 0, width, height);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_TEXTURE_2D);   
    glEnable(GL_POINT_SMOOTH); 
    glEnable(GL_SCISSOR_TEST);

    ReleaseDC(windowHandle, hdc);
    


    surface->size = { width, height };
    surface->backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

    return surface;

}

void uRenderSurface::SizeChanged(float width, float height) {

    size = { width, height };

    HDC hdc = GetDC(resources.windowHandle);
    wglMakeCurrent(hdc, resources.glRenderContextHandle);
    glViewport(0, 0, width, height);

    wglMakeCurrent(NULL, NULL); 
    ReleaseDC(resources.windowHandle, hdc); // Release the device context
}



void uRenderSurface::Render() {
    
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(resources.windowHandle, &ps);

    wglMakeCurrent(hdc, resources.glRenderContextHandle);

    glViewport(0, 0, size.width, size.height);
    glScissor(0, 0, size.width, size.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, size.width, size.height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear screen
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    for (uCanvas* canvas : canvasList) {

        if (canvas->clipsToBounds) {
            // flip y coordinate
            glScissor(canvas->x, size.height - canvas->y - canvas->height, canvas->width, canvas->height);
        } else {
            glScissor(0, 0, size.width, size.height);
        }

        RenderCommands(canvas, canvas->commands);
    }
    // do other rendering here

    SwapBuffers(hdc);
    EndPaint(resources.windowHandle, &ps);

}

void RenderCommands(uCanvas* canvas, std::vector<uDrawingCommand>& commands) {

    float originX = canvas->x;
    float originY = canvas->y;

    glClearColor(canvas->backgroundColor.r, canvas->backgroundColor.g, canvas->backgroundColor.b, canvas->backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);


    for (uDrawingCommand command : commands) {
        //printf("DRAWING COMMAND %d %f %f %f %f\n", command.commandType, command.parameterA, command.parameterB, command.parameterC, command.parameterD );

        switch (command.commandType) {
            case BEGIN_LINES: {
                    glBegin(GL_LINES);
                    break;
                }
            case END_LINES: {
                    glEnd();
                    break;
                }
            case VERTEX_2D: {
                    glVertex2d(originX + command.parameterA, originY + command.parameterB);
                    break;
                }
            case COLOR_4F: {
                    glColor4f(command.parameterA, command.parameterB, command.parameterC, command.parameterD);
                    break;
                }
            case LINE_WIDTH: {
                    glLineWidth(command.parameterA);
                    break;
                }

            default: 
                break;
        }
    }
}
#include "Graphics/Surface/uRenderSurface.h"

#include <chrono>

const float budgetUs = 1000000.0f/144.0f;

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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction for text

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

    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

    
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
                            
    std::chrono::nanoseconds elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    float proportionOfBudget = (float) microseconds / budgetUs;
    printf("RENDER TOOK %lld us (%.12f %) \n", microseconds, proportionOfBudget*100.0f);

    SwapBuffers(hdc);
    EndPaint(resources.windowHandle, &ps);

}

void RenderCommands(uCanvas* canvas, std::vector<uDrawingCommand>& commands) {

    float originX = canvas->x;
    float originY = canvas->y;

    //printf("background color %f %f %f %f\n", canvas->backgroundColor.r, canvas->backgroundColor.g, canvas->backgroundColor.b, canvas->backgroundColor.a);

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
            case BEGIN_POLYGON: {
                    glBegin(GL_POLYGON);
                    break;
                }
            case END_POLYGON: {
                    glEnd();
                    break;
                }
            case BEGIN_QUADS: {
                    glBegin(GL_QUADS);
                    break;
                }
            case END_QUADS: {
                    glEnd();
                    break;
                }
            case BEGIN_TEXTURE: {
                    glBindTexture(GL_TEXTURE_2D, command.parameterE);
                    break;
                }
            case END_TEXTURE: {
                    glBindTexture(GL_TEXTURE_2D, 0);
                    break;
                }
            
            case VERTEX_2D: {
                    //printf("VERTEX2D %f %f\n", originX + command.parameterA, originY + command.parameterB);
                    glVertex2d(originX + command.parameterA, originY + command.parameterB);
                    break;
                }
            case TEX_VERTEX_2D: {
                    glTexCoord2d(command.parameterA, command.parameterB);
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
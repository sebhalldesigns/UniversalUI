//
// Created by sebam on 11/11/2023.
//

#include "../include/uWindowManager.h"

#include "uCanvas.h"
#include "uRenderNode.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "glm/glm.hpp"

std::vector<uWindowStuffForManager> uWindowManager::windows;

#include <cstdio>
#include <map>

#ifdef _WIN32
    const wchar_t CLASS_NAME[]  = L"UniversalUI Window";

    LRESULT CALLBACK Win32WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    FT_Library ft;
    struct Character {
        GLuint textureID;  // Texture ID
        glm::ivec2 size;   // Size of glyph
        glm::ivec2 bearing;// Offset from baseline to left/top of glyph
        FT_Pos advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;

#elif __linux__
    Display *display;
    Window root;
    XWindowAttributes windowAttributes;


    void X11WindowProcedure(XEvent event);

    int X11HandleError(Display* display, XErrorEvent* event);
#elif __APPLE__
    #include <Cocoa/Cocoa.h>
    #include <OpenGL/gl.h>

    WindowDelegate* uWindowManager::nsWindowDelegate;


    enum WindowEvent {
        SIZE
    };

    void CocoaWindowProcedure(WindowEvent event, NSWindow* window, double param1, double param2);

    @implementation WindowDelegate
    - (NSSize)windowWillResize:(NSWindow *)sender 
                    toSize:(NSSize)frameSize {

        CocoaWindowProcedure(WindowEvent::SIZE, sender, frameSize.width, frameSize.height);
        return frameSize;
    }
    @end
#endif


bool uWindowManager::Init() {


#ifdef _WIN32

    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc   = Win32WindowProcedure;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == 0) {
        return false;
    }
#elif __linux__

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (!display) {
        printf("Unable to open X display. Exiting...\n");
        return false;
    }

    // Set up error handler for X errors
    XSetErrorHandler(X11HandleError);

    // Get the root window
    root = DefaultRootWindow(display);

#elif __APPLE__
    nsWindowDelegate = [[WindowDelegate alloc] init];
#endif

   
    return true;
}

void uWindowManager::Close() {
    #ifdef _WIN32
    #elif __linux__
        XCloseDisplay(display);
    #endif
}


void uWindowManager::PollEvents() {
    #ifdef _WIN32
        // Run the message loop.
        MSG msg = { };
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    #elif __linux__

        XEvent xEvent;

        while (XPending(display)) {
            XNextEvent(display, &xEvent);
            X11WindowProcedure(xEvent);
        }

    #endif
}

bool uWindowManager::IsWindowsEmpty() {
    return windows.empty();
}

bool uWindowManager::WindowStuffFromSystemHandle(uWindowHandle handle, uWindowStuffForManager& windowStuff) {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].windowPointer->systemHandle == handle) { 
            windowStuff = windows[i];
            return true;
        }
    }

    return false;
}

void uWindowManager::SetWindowSize(uWindow* window, double width, double height) {
    window->width = width;
    window->height = height;
}

void uWindowManager::GetWindowSize(uWindow* window, double& width, double& height) {
    width = window->width;
    height = window->height;
}


void uWindowManager::CreateNewWindow(uWindow* window, double width, double height, std::string title) {

    uWindowStuffForManager stuffForManager;

    #ifdef _WIN32
        std::wstring wideTitle = std::wstring(title.begin(), title.end());

        window->systemHandle = CreateWindowExW(
                0,                              // Optional window styles.
                CLASS_NAME,                     // Window class
                wideTitle.c_str(),  // Window text
                WS_OVERLAPPEDWINDOW,            // Window style

                // Size and position
                CW_USEDEFAULT, CW_USEDEFAULT, (int)width, (int)height,


                nullptr,       // Parent window
                nullptr,       // Menu
                GetModuleHandleW(nullptr),  // Instance handle
                nullptr        // Additional application data
        );

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

        if (window->systemHandle == nullptr)
        {
            printf("ERROR: Window creation failed!");
            return;
        }

        stuffForManager.windowPointer = window;

        HDC hdc = GetDC(window->systemHandle);
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);
        stuffForManager.glRenderContextHandle = wglCreateContext(hdc);
        wglMakeCurrent(hdc, stuffForManager.glRenderContextHandle);

        
        glEnable(GL_BLEND);  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        glEnable(GL_TEXTURE_2D);   
        glEnable(GL_POINT_SMOOTH); 

         if (FT_Init_FreeType(&ft)) {
            // Handle error: initialization failed
        }

        FT_Face face;
        if (FT_New_Face(ft, "./font.ttf", 0, &face)) {
            // Handle error: failed to load font
        }

        // Set the desired font size
        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (GLubyte c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                // Handle error: failed to load glyph
                continue;
            }

            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character for later use
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }

    glBindTexture(GL_TEXTURE_2D, 0);



        ReleaseDC(window->systemHandle, hdc);

        windows.push_back(stuffForManager);

        printf("Window %x created\n", window->systemHandle);

        //uRenderManager::SetupForWindow(window);
        //uRenderManager::RenderToWindow(window);Expose
    #elif __linux__

        // Find a suitable visual
        int attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        stuffForManager.visualInfo = glXChooseVisual(display, 0, attribs);
        if (!stuffForManager.visualInfo) {
            printf("Unable to find a suitable visual. Exiting...\n");
            return;
        }

        // Create colormap
        stuffForManager.colormap = XCreateColormap(display, root, stuffForManager.visualInfo->visual, AllocNone);

        // Set up window attributes
        stuffForManager.setWindowAttributes.colormap = stuffForManager.colormap;
        stuffForManager.setWindowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | SubstructureNotifyMask;

        // NOTE the pointer to stuffForManager.setWindowAttributes may break
        window->systemHandle = XCreateWindow(
            display, root, 0, 0, width, height, 0, stuffForManager.visualInfo->depth, InputOutput,
            stuffForManager.visualInfo->visual, CWColormap | CWEventMask, &stuffForManager.setWindowAttributes
        );

        if (window->systemHandle == NULL) {
            printf("ERROR: Window creation failed!\n");
            return;
        }

        stuffForManager.windowPointer = window;

        // Set the window title
        XStoreName(display, window->systemHandle, title.c_str());

        // Link the delete button atom
        // Set the WM_DELETE_WINDOW protocol for the close button
        stuffForManager.deleteButtonAtom = XInternAtom(display, "WM_DELETE_WINDOW", False); 
        XSetWMProtocols(display, window->systemHandle, &stuffForManager.deleteButtonAtom, 1);


        // Create GLX context
        stuffForManager.glxContext = glXCreateContext(display, stuffForManager.visualInfo, NULL, GL_TRUE);
        if (!stuffForManager.glxContext) {
            printf("Failed to create GLX context. Exiting...\n");
            return;
        }

        // Make the GLX context current
        glXMakeCurrent(display, window->systemHandle, stuffForManager.glxContext);

        glEnable(GL_BLEND);  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        glEnable(GL_TEXTURE_2D);   
        glEnable(GL_POINT_SMOOTH); 

        // Show the window
        XMapWindow(display, window->systemHandle);

        windows.push_back(stuffForManager);

    #elif __APPLE__

        NSRect frame = NSMakeRect(0, 0, width, height);
        window->systemHandle = [[NSWindow alloc] initWithContentRect: frame 
                                styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
                                backing:NSBackingStoreBuffered
                                defer:NO];

        if (window->systemHandle == nullptr) {
            printf("ERROR: Window creation failed!");
            return;
        }

        stuffForManager.windowPointer = window;

        [window->systemHandle setDelegate: nsWindowDelegate];
        [window->systemHandle setTitle: [[NSString alloc] initWithUTF8String: title.c_str()]];

        NSOpenGLPixelFormatAttribute attrs[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 16,
            NSOpenGLPFADoubleBuffer,
            0
        };

        stuffForManager.pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        stuffForManager.glContext = [[NSOpenGLContext alloc] initWithFormat: stuffForManager.pixelFormat shareContext:nil];
        [stuffForManager.glContext makeCurrentContext];
        glEnable(GL_BLEND);  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        glEnable(GL_TEXTURE_2D);   
        glEnable(GL_POINT_SMOOTH); 


        [window->systemHandle.contentView setWantsLayer:YES]; // For layer-backed views
        [stuffForManager.glContext setView:window->systemHandle.contentView];

        [window->systemHandle makeKeyAndOrderFront:nil];


        windows.push_back(stuffForManager);
        printf("Created a window!\n");
    #endif

}

void uWindowManager::DestroyWindowByHandle(uWindowHandle handle) {

    uWindowStuffForManager windowStuff;
    if (!uWindowManager::WindowStuffFromSystemHandle(handle, windowStuff)) {
        printf("ERROR: Unknown window!\n");
        return;
    }

    #ifdef _WIN32

        for (int i = 0; i < windows.size(); i++) {
            if (windows[i].windowPointer->systemHandle == handle) { 
                if (DestroyWindow(handle)) { 
                    // Cleanup and exit
                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(windows[i].glRenderContextHandle);
                    windows.erase(windows.begin() + i);
                    return;
                }
            }
        }
    #elif __linux__
        for (int i = 0; i < windows.size(); i++) {
            if (windows[i].windowPointer->systemHandle == handle) { 
                // Cleanup and exit
                glXMakeCurrent(display, None, NULL);
                glXDestroyContext(display, windowStuff.glxContext);
                XDestroyWindow(display, handle);
                windows.erase(windows.begin() + i);
                return;
            }
        }
        return;

    #endif
}




void uWindowManager::SetWindowVisibility(uWindow* window, uWindowVisibility visibility) {

    #ifdef _WIN32
        switch (visibility) {
            case uWindowVisibility::VISIBLE:
                ShowWindow(window->systemHandle, SW_SHOW);
                break;
            case uWindowVisibility::MINIMISED:
                ShowWindow(window->systemHandle, SW_SHOWMINIMIZED);
                break;
            default:
                ShowWindow(window->systemHandle, SW_HIDE);
                break;
        }
    #endif // _WIN32
}


void RenderView(uView* view) {

    printf("RENDER VIEW\n");

    // Set color for the rectangle (for example, green)
    uRectangle frame = view->layoutNode->frame;
    // Draw rectangle based on view's frame
    glBegin(GL_QUADS);
        glColor4f(view->backgroundColor.r, view->backgroundColor.g, view->backgroundColor.b, view->backgroundColor.a); // Green color
        glVertex2f(frame.x, frame.y);
        glVertex2f(frame.x + frame.width, frame.y);
        glVertex2f(frame.x + frame.width, frame.y + frame.height);
        glVertex2f(frame.x, frame.y + frame.height);
    glEnd();

    uCanvas canvas(frame.width, frame.height);

    view->Draw(canvas);

    uRenderNode renderNode;
    renderNode.UpdateRenderCommands(canvas);

    for (uRenderCommand renderCommand : renderNode.renderCommands) {
        switch (renderCommand.commandType) {
            case BEGIN_POINTS: {
                    glBegin(GL_POINTS);
                    break;
                }
            case END_POINTS: {
                    glEnd();
                    break;
                }
            case VERTEX_2D: {
                    glVertex2d(renderCommand.parameters[0], renderCommand.parameters[1]);
                    break;
                }
            case POINT_SIZE: {
                    glPointSize(renderCommand.parameters[0]);
                    break;
                }
            case COLOR_4F: {
                    glColor4d(renderCommand.parameters[0], renderCommand.parameters[1], renderCommand.parameters[2], renderCommand.parameters[3]);
                    break;
                }
        }
    }
    
    for (uView* subview : view->subviews) {
        RenderView(subview);
    }
}

void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    // Activate corresponding render state	


    glPushMatrix();
    glTranslatef(x, y, 0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBegin(GL_QUADS);
            glColor3f(color.x, color.y, color.z);
            glTexCoord2d(0, 0); glVertex2f(xpos, ypos);
            glTexCoord2d(1, 0); glVertex2f(xpos + w, ypos);
            glTexCoord2d(1, 1); glVertex2f(xpos + w, ypos + h);
            glTexCoord2d(0, 1); glVertex2f(xpos, ypos + h);
        glEnd();

        // Now advance cursors for next glyph
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times)
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}



#ifdef _WIN32

LRESULT CALLBACK Win32WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            uWindowManager::DestroyWindowByHandle(hwnd);
            printf("Window %x close\n", hwnd);

            return 0;
        case WM_SIZE: {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                for (int i = 0; i < uWindowManager::windows.size(); i++) {
                    if (uWindowManager::windows[i].windowPointer->systemHandle == hwnd) {
                        HDC hdc = GetDC(hwnd); // Get the device context for the window
                        wglMakeCurrent(hdc, uWindowManager::windows[i].glRenderContextHandle);
                        glViewport(0, 0, width, height);
                        wglMakeCurrent(NULL, NULL); // Optionally, make no context current  

                        printf("RESIZE %d %d\n", width, height);

                        ReleaseDC(hwnd, hdc); // Release the device context
                        InvalidateRect(hwnd, NULL, FALSE); // Request a redraw

                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.frame.width = width;
                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.frame.height = height;

                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.EvaluateConstraints();
                    }
                }
                
                return 0;
            }


        case WM_PAINT: {

            
            for (int i = 0; i < uWindowManager::windows.size(); i++) {
                if (uWindowManager::windows[i].windowPointer->systemHandle == hwnd) {

                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    wglMakeCurrent(hdc, uWindowManager::windows[i].glRenderContextHandle);


                    // Set up OpenGL viewport, projection, etc.
                    glViewport(0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    glOrtho(0.0, ps.rcPaint.right, ps.rcPaint.bottom, 0.0, -1.0, 1.0);
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();

                    // Clear screen
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    RenderView(&uWindowManager::windows[i].windowPointer->rootView);

                    RenderText("Hello World!", 25.0f, 50.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

                    SwapBuffers(hdc);

                    EndPaint(hwnd, &ps);
                }

            
            }
        

            return 0;
        }

        case WM_DISPLAYCHANGE: {
            InvalidateRect(hwnd, NULL, FALSE); // Request a redraw
            return 0;
        }
            
        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#elif __linux__

void X11WindowProcedure(XEvent event) {

    switch (event.type) { 
        case ClientMessage: {
                Window xWindow = event.xclient.window; 
                uWindowManager::DestroyWindowByHandle(xWindow);

                // Check if it's a WM_DELETE_WINDOW message
                /*if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", False) &&
                        event.xclient.data.l[0] == deleteButtonAtom) {
                    uWindowManager::DestroyWindowByHandle(xWindow);
                }*/
                return;
            }
            
        case Expose: {
                Window xWindow = event.xexpose.window; 
                // Handle expose event

                uWindowStuffForManager windowStuff;
                if (!uWindowManager::WindowStuffFromSystemHandle(xWindow, windowStuff)) {
                    printf("ERROR: Unknown window!\n");
                    return;
                }


                // get window attributes e.g width, ehgit
                XGetWindowAttributes(display, xWindow, &windowAttributes);

                // Make the GLX context current
                glXMakeCurrent(display, xWindow, windowStuff.glxContext);

                

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0.0, windowAttributes.width, windowAttributes.height, 0.0, -1.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();


                // Define the solid color (RGB)
                float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};  // Blue
                glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
                glClear(GL_COLOR_BUFFER_BIT);


                RenderView(&windowStuff.windowPointer->rootView);
                glXSwapBuffers(display, xWindow);

                return;
            }
        /* case MapNotify: {
                Window xWindow = event.xexpose.window; 
                // Handle expose event

                uWindowStuffForManager windowStuff;
                if (!uWindowManager::WindowStuffFromSystemHandle(xWindow, windowStuff)) {
                    printf("ERROR: Unknown window!\n");
                    return;
                }


                // Make the GLX context current
                glXMakeCurrent(display, xWindow, windowStuff.glxContext);

                // Define the solid color (RGB)
                float clearColor[] = {0.0f, 0.0f, 1.0f, 1.0f};  // Blue
                glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
                glClear(GL_COLOR_BUFFER_BIT);

                glXSwapBuffers(display, xWindow);

                return;
            }*/
        case ConfigureNotify: {
                Window xWindow = event.xexpose.window; 

                uWindowStuffForManager windowStuff;
                if (!uWindowManager::WindowStuffFromSystemHandle(xWindow, windowStuff)) {
                    printf("ERROR: Unknown window!\n");
                    return;
                }

                // get window attributes e.g width, ehgit
                XGetWindowAttributes(display, xWindow, &windowAttributes);

                // Make the GLX context current
                glXMakeCurrent(display, xWindow, windowStuff.glxContext);
                glViewport(0, 0, windowAttributes.width, windowAttributes.height);

                windowStuff.windowPointer->layoutTree.rootNode.frame.width = windowAttributes.width;
                windowStuff.windowPointer->layoutTree.rootNode.frame.height = windowAttributes.height;

                windowStuff.windowPointer->layoutTree.rootNode.EvaluateConstraints();

                // TODO refactor the resizing and layout and drawing code

                uWindowManager::SetWindowSize(windowStuff.windowPointer, windowAttributes.width, windowAttributes.height);
            
                return;
            }
        case DestroyNotify: {
                return;
            }
        case UnmapNotify: {
                return;
            }
        case ReparentNotify: {
                return;
            }
        default: {
                printf("ERROR: Unhandled XEvent %u\n", event.type);
                return;
            }
            
    }
}

int X11HandleError(Display* display, XErrorEvent* event) {
    printf("X Error: An error occurred. Exiting...\n");
    //exit(EXIT_FAILURE);
    return 0;
}

#elif __APPLE__

void CocoaWindowProcedure(WindowEvent event, NSWindow* window, double param1, double param2) {
    switch (event) {
        case SIZE: {
                uWindowStuffForManager windowStuff;
                if (!uWindowManager::WindowStuffFromSystemHandle(window, windowStuff)) {
                    printf("ERROR: Unknown window!\n");
                    return;
                }

                [windowStuff.glContext makeCurrentContext];

                // Set up OpenGL viewport, projection, etc.
                glViewport(0, 0, param1, param2);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0.0, param1, param2, 0.0, -1.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

                glClearColor(0.0, 0.5, 1.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);

                windowStuff.windowPointer->layoutTree.rootNode.frame.width = param1;
                windowStuff.windowPointer->layoutTree.rootNode.frame.height = param2;

                windowStuff.windowPointer->layoutTree.rootNode.EvaluateConstraints();
                RenderView(&windowStuff.windowPointer->rootView);

                [windowStuff.glContext flushBuffer];

                printf("NEW SIZE: %f %f\n", param1, param2);
                return;
            }
        default: {
                printf("ERROR: Unhandled event %u \n", event);
                return;
            }

    }
}
#endif

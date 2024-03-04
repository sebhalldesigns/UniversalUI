#include "uWindow.h"

#include <cstdio>

uWindow::uWindow(std::string title, uSize size) {
    printf("uWindow Init\n");
    gCreateWindow(this->windowHandle, title, size);
    gSetWindowVisibility(this->windowHandle, gWindowVisibility::VISIBLE);

    gSetWindowResizeCallback(this->windowHandle, &this->ResizeCallback);

}

void uWindow::ResizeCallback(gWindow window, uSize newSize) { 

}

void uWindow::MoveCallback(uPoint newLocation) { 

}

void uWindow::VisibilityChangedCallback(uWindowVisibility visibility) { 
    
}

void uWindow::FocusChangedCallback(uWindowFocus focus) { 
    
}

void uWindow::DrawCallback() { 

}

void uWindow::CloseCallback() { 

}

uSize uWindow::ResizeRequestCallback(uSize sizeRequest) {
    return sizeRequest;
}

uPoint uWindow::MoveRequestCallback(uPoint locationRequest) {
    return locationRequest;
}

bool uWindow::CloseRequestCallback() {
    return true;
}
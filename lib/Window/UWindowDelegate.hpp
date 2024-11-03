#ifndef UWINDOWDELEGATE_HPP
#define UWINDOWDELEGATE_HPP

#include <Geometry/USize.hpp>

#include <vector>


class UWindow;

class UWindowDelegate
{

public:
    UWindowDelegate() = default;
    ~UWindowDelegate() = default;
    
    virtual void WindowWillAppear(UWindow* window) { }
    
    virtual bool WindowShouldClose(UWindow *window) { return true; }
    virtual void WindowWillClose(UWindow *window) { }

    virtual USize WindowWillResize(UWindow *window, USize size) { return size; }
    virtual void WindowDidResize(UWindow *window, USize size) { }

};

#endif // UWINDOWDELEGATE_HPP
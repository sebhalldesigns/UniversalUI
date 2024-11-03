#ifndef UAPPLICATIONDELEGATE_HPP
#define UAPPLICATIONDELEGATE_HPP

#include <vector>

#include <Window/UWindowCreateInfo.hpp>


class UApplication;
class UWindow;

class UApplicationDelegate
{

public:
    UApplicationDelegate() = default;
    ~UApplicationDelegate() = default;

    virtual std::vector<UWindowCreateInfo> ApplicationWillCreateWindows(UApplication *application) { return std::vector<UWindowCreateInfo>(); }
    
    virtual void ApplicationDidFinishLaunching(UApplication *application, UWindow* window) = 0;
    virtual void ApplicationWillTerminate(UApplication *application) = 0;

};

#endif // UAPPLICATIONDELEGATE_HPP
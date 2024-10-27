#ifndef UAPPLICATIONDELEGATE_HPP
#define UAPPLICATIONDELEGATE_HPP

class UApplication;
class UWindow;

class UApplicationDelegate
{

public:
    UApplicationDelegate() = default;
    ~UApplicationDelegate() = default;

    virtual void ApplicationDidFinishLaunching(UApplication *application, UWindow* window) = 0;
    virtual void ApplicationWillTerminate(UApplication *application) = 0;
};

#endif // UAPPLICATIONDELEGATE_HPP
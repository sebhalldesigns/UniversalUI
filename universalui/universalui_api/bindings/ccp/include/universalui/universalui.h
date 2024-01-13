#ifndef UNIVERSALUI_H
#define UNIVERSALUI_H

#define DLIB __declspec(dllimport)

#include <stdint.h>

struct uSize {
    float width;
    float height;
};

typedef void (*uApplicationLaunchedCallback)();
typedef bool (*uApplicationShouldQuitCallback)();
typedef void (*uApplicationWillQuitCallback)();

typedef void (*uWindowWillResizeCallback)(uSize size);

struct uVersion {
    uint32_t major;
    uint32_t minor;
    uint32_t build;    
};



struct uApplication {
     char* title;
    const char* developer;
    uVersion version;
    uApplicationLaunchedCallback launched_callback = NULL;
    uApplicationShouldQuitCallback should_quit_callback = NULL;
    uApplicationWillQuitCallback will_quit_callback = NULL;
};

struct uWindow {
    void* raw_handle;
    char* title;
    uSize size;
    uWindowWillResizeCallback will_resize = NULL;
};

extern "C" int UniversalUI(uApplication app);

extern "C" uWindow* CreateWindow(const char* title, float width, float height);



#endif // UNIVERSALUI_H
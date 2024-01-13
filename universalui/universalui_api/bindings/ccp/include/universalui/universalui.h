#ifndef UNIVERSALUI_H
#define UNIVERSALUI_H

#define DLIB __declspec(dllimport)

#include <stdint.h>

typedef void (*uApplicationLaunchedCallback)();
typedef bool (*uApplicationShouldQuitCallback)();
typedef void (*uApplicationWillQuitCallback)();

struct uVersion {
    uint32_t major;
    uint32_t minor;
    uint32_t build;    
};

struct uApplication {
    const char* title;
    const char* developer;
    uVersion version;
    uApplicationLaunchedCallback launched_callback;
    uApplicationShouldQuitCallback should_quit_callback;
    uApplicationWillQuitCallback will_quit_callback;
};

extern "C" int UniversalUI(uApplication app);

extern "C" int CreateWindow(const char* title, int width, int height);



#endif // UNIVERSALUI_H
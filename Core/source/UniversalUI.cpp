//
// Created by sebam on 11/11/2023.
//

#include "../include/UniversalUI.h"

#include <stdio.h>

#ifdef __APPLE__
    #include <Cocoa/Cocoa.h>
#endif

int UniversalUI(uApplication* application) {

    #ifdef __APPLE__
        @autoreleasepool {
            [NSApplication sharedApplication];
        
    #endif

    if (!uWindowManager::Init()) {
        return -1;
    }

    application->FinishedLaunching();

    #ifdef __APPLE__

            printf("APP RUN!");
            // Create and run the application event loop
            NSApp.activationPolicy = NSApplicationActivationPolicyRegular;
            [NSApp activateIgnoringOtherApps:YES];
            [NSApp run];

        } // @autoreleasepool
    #else

        while (!application->shouldQuit) {

            uWindowManager::PollEvents();

            if (uWindowManager::IsWindowsEmpty() && application->quitWhenLastWindowClosed) {
                printf("\nINFO: Application finishing due to no windows being present\n");
                application->shouldQuit = true;
            }
        }

    #endif
   

    return 0;
}
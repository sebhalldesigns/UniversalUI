//
// Created by sebam on 11/11/2023.
//

#include "../include/UniversalUI.h"



#include <stdio.h>

int UniversalUI(uApplication* application) {

    if (!uWindowManager::Init()) {
        return -1;
    }

    application->FinishedLaunching();

    while (!application->shouldQuit) {

        uWindowManager::PollEvents();

        if (uWindowManager::IsWindowsEmpty() && application->quitWhenLastWindowClosed) {
            printf("\nINFO: Application finishing due to no windows being present\n");
            application->shouldQuit = true;
        }
    }

    return 0;
}
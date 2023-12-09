//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOWMANAGER_H
#define UWINDOWMANAGER_H

#include "nWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>

class uWindowManager {
public:

    static std::vector<nWindow> windows;

    static bool Init();
    static void Close();

    static void PollEvents();

    static bool IsWindowsEmpty();

    static void SetWindowSize(nWindow* window, double width, double height);
    static void GetWindowSize(nWindow* window, double& width, double& height);

    static void CreateNewWindow(nWindow* window, double width, double height, std::string title);
    //static void SetWindowVisibility(nWindow* window, uWindowVisibility visibility);
};

#endif //UWINDOWMANAGER_H
//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "Core/Application/uApplication.h"
#include "Core/Windowing/uWindow.h"

#include <random>


class MyWindow : public uWindow {
public:

    void OnCreated() override {
        printf("Window created!\n");

    }
};


class MyApp : public uApplication {

    void FinishedLaunching() override {

        printf("started launching from windows\n");

        uWindow* window = NewWindow(800, 600, "My Window");

    }
};

int main() {

    MyApp app;
    return app.Run();
}

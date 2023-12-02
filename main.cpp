//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "Core/UniversalUI.h"
#include "Core/Windowing/uWindow.h"

#include <random>



class MyApp : public uApplication {

    void FinishedLaunching() override {

        printf("started launching from macos!!\n");

    }
};

class MyWindow : public uWindow {
public:

    void OnCreated() override {
        printf("Window created!\n");
    }
};

int main() {

    MyApp* app = new MyApp();
    app->simpleWindow = new MyWindow();
    return UniversalUI(app);
}

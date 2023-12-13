//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "Core/Application/uApplication.h"
#include "Core/Windowing/uWindow.h"
#include "Core/uView.h"

#include "Graphics/Canvas/uDrawingItem.h"

#include <random>


class MyWindow : public uWindow {
public:

    void OnCreated() override {
        printf("Window created!\n");

    }
};

class uButton : public uView {

public:

    void Draw(uCanvas& canvas) override { 
        //printf("draw button!!\n");

        Draw::RoundedRect rect;
        rect.origin = { 10.0f, 10.0f };
        rect.size = { frame.width - 20.0f, frame.height - 20.0f };
        rect.fillColor = { 0.2f, 0.2f, 0.2f, 1.0f };
        rect.cornerRadius = 10.0f;
        canvas.AddItem(rect);
    
        /*
        Draw::Line line;
        line.start = { 10.0f, 10.0f };
        line.end = { frame.width - 10.0f, frame.height - 10.0f };
        line.color = { 1.0f, 0.0f, 0.0f, 1.0f };
        line.thickness = 5.0f;

        canvas.AddItem(line);
        */
    }

};


class MyApp : public uApplication {

    void FinishedLaunching() override {

        printf("started launching from windows\n");

        uWindow* window = NewWindow(800, 600, "My Window");

        uButton* button = new uButton;
        button->frame = { 10.0f, 10.0f, 100.0f, 100.0f };
        window->rootView.AddSubview(button);

        button->AddConstraint(&window->rootView, uLayoutAnchor::LEFT, uLayoutAnchor::LEFT, 0.0f);
        button->AddConstraint(&window->rootView, uLayoutAnchor::TOP, uLayoutAnchor::TOP, 0.0f);
        button->AddConstraint(&window->rootView, uLayoutAnchor::BOTTOM, uLayoutAnchor::BOTTOM, 0.0f);
        button->AddConstraint(&window->rootView, uLayoutAnchor::WIDTH, uLayoutAnchor::WIDTH, 0.0f, 0.5f);
    }
};

int main() {

    MyApp app;
    return app.Run();
}

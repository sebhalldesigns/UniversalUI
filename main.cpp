//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "Core/Application/uApplication.h"
#include "Core/Windowing/uWindow.h"
#include "Core/uView.h"

#include "Graphics/Canvas/uDrawingItem.h"
#include "Graphics/Text/uFont.h"

#include <random>

#include "uRmlParser.h"

uFont* myfont;

class MyWindow : public uWindow {
public:

    void OnCreated() override {
        printf("Window created!\n");

    }
};

class Button : public uView {

public:

    void Draw(uCanvas& canvas) override { 
        //printf("draw rectangle!!\n");

        Draw::RoundedRect rect;
        rect.origin = { 0.0f, 0.0f };
        rect.size = { frame.width, frame.height };
        rect.fillColor = { 0.1f, 0.1f, 0.1f, 1.0f };
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

    void MouseEnter() override {
        backgroundColor = { 1.0f, 0.0f, 0.0f, 1.0f};
        printf("MOUSE ENTER\n");
    }

    void MouseExit() override {
        backgroundColor = { 1.0f, 1.0f, 1.0f, 1.0f};
        printf("MOUSE EXIT\n");
    }

};


class LeftRectangle : public uView {

public:

    void Draw(uCanvas& canvas) override { 
        backgroundColor = { 1.0f, 1.0f, 1.0f, 1.0f};

        //printf("draw rectangle!!\n");
        /*
        Draw::RoundedRect rect;
        rect.origin = { 10.0f, 10.0f };
        rect.size = { frame.width - 20.0f, frame.height - 20.0f };
        rect.fillColor = { 0.2f, 0.2f, 0.2f, 1.0f };
        rect.cornerRadius = 10.0f;
        canvas.AddItem(rect);
        */
        
        Draw::Label label;
        label.origin = { 100.0f, 100.0f };
        label.size = { 2000.0f, 0.0f };
        label.canBeTaller = true;
        label.canBeWider = true;
        label.text = "Hello world";
        label.fontColor = { 0.0f, 0.0f, 0.0f, 1.0f};
        label.font = myfont;
        canvas.AddItem(label);

        canvas.clipsToBounds = false;

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

        printf("started launching from windows 노래방 六書; 六书 الْأَبْجَدِيَّة الْعَرَبِيَّة 送り仮名 👋\n");

        uWindow* window = NewWindow(800, 600, "My Window 노래방 六書; 六书 الْأَبْجَدِيَّة الْعَرَبِيَّة 送り仮名 👋");

        LeftRectangle* rectangle = new LeftRectangle;
        rectangle->frame = { 10.0f, 10.0f, 100.0f, 100.0f };
        window->rootView.AddSubview(rectangle);

        rectangle->AddConstraint(&window->rootView, uLayoutAnchor::LEFT, uLayoutAnchor::LEFT, 0.0f);
        rectangle->AddConstraint(&window->rootView, uLayoutAnchor::TOP, uLayoutAnchor::TOP, 0.0f);
        rectangle->AddConstraint(&window->rootView, uLayoutAnchor::BOTTOM, uLayoutAnchor::BOTTOM, 0.0f);
        rectangle->AddConstraint(&window->rootView, uLayoutAnchor::WIDTH, uLayoutAnchor::WIDTH, 0.0f, 0.5f);
        /*
        for (int x = 0; x < 45; x++) {
            for (int y = 0; y < 25; y++) {
                Button* button = new Button;
                button->frame = { 20.0f + x*55.0f , 20.0f + y*55.0f , 50.0f, 50.0f};
                window->rootView.AddSubview(button);
            }


        }*/
    }
};

int main() {

    MyApp app;

    uRmlParser::ParseFile("../../main.xml");

    return 0;
    //myfont = new uFont("./font.ttf");
    //return app.Run();


}

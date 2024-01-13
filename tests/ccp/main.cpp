#include <universalui.h>
#include <stdio.h>

void Launched() {
    printf("launched!\n");
}

bool ShouldQuit() {
    return true;
}

void WillQuit() {

}


int main() {

    uApplication app;
    app.developer = "example person";
    app.title = "my app";
    app.version = { 0, 0, 0};
    app.launched_callback = Launched;
    app.should_quit_callback = ShouldQuit;
    app.will_quit_callback = WillQuit;


    return UniversalUI(app);
}
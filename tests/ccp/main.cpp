#include <universalui.h>
#include <stdio.h>

void Resize(uSize size) {
    printf("resize in c! %f %f\n", size.width, size.height);
}

void Launched() {
    //printf("launched!\n");

    uWindow* window = CreateWindow("Hello UniversalUI 천이백삼십사", 800, 600);
    window->will_resize = Resize;
    printf("finished launching!\n");
    window->title = "title";
    printf("%llu %llu\n", window, &window->will_resize);
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
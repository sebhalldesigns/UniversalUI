#include "GspSystem.h"
#include "GspGraphics.h"

#include "Types/uWindow.h"

void ApplicationLaunched(std::vector<std::string> args) {
    printf("APPLICATION LAUNCHED FROM UNIVERSALUI\n");

    //gWindow window;
    //gCreateWindow(window, "Hello UniversalUI", { 1000.0, 750.0 });
    //gSetWindowVisibility(window, gWindowVisibility::VISIBLE);

    uWindow window("new window", uSize { 800.0f, 600.0f });

}


int main() {

    gApplicationInfo appInfo{};
    appInfo.quitWhenNoWindows = true;

    gApplicationSetLaunchedCallback(ApplicationLaunched);

    return gApplicationRun(appInfo);
}
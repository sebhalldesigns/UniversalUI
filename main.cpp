#include "GspSystem.h"
#include "GspGraphics.h"

void ApplicationLaunched(std::vector<std::string> args) {
    printf("APPLICATION LAUNCHED FROM UNIVERSALUI\n");

    gWindow window;
    gCreateWindow(window, "Hello UniversalUI", { 1000.0, 750.0 });
    gSetWindowVisibility(window, gWindowVisibility::VISIBLE);

}


int main() {

    gApplicationInfo appInfo{};
    appInfo.quitWhenNoWindows = true;

    gApplicationSetLaunchedCallback(ApplicationLaunched);

    return gApplicationRun(appInfo);
}
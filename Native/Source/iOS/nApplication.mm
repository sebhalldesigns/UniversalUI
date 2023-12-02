#include "Native/Application/nApplication.h"
#include <cstdio>
#include <stdexcept>

#import <UIKit/UIKit.h>

static uApplication* app;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@end

@implementation AppDelegate 


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
        printf("app delegate launched with an update and another update!\n");

        if (app == nullptr) {
            printf("[CRITICAL] Invalid uApplication instance!\n");
            exit(EXIT_FAILURE);
        }


        app->FinishedLaunching();
        
        if (app->simpleWindow == nullptr) {
            printf("[CRITICAL] No simple window set!\n");
            exit(EXIT_FAILURE);
        }
    
        app->simpleWindow->OnCreated();

    }

@end

int nApplication::Run(uApplication* application) {
    app = application;
    @autoreleasepool {
        return UIApplicationMain(0, nil, nil, NSStringFromClass([AppDelegate class]));
    }
    
}

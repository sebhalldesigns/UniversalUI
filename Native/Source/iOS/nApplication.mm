#include "Native/Application/nApplication.h"
#include <cstdio>

#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@end




@implementation AppDelegate 


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    printf("app delegate launched!\n");
}

@end

int nApplication::Run(uApplication* application) {

    @autoreleasepool {
        return UIApplicationMain(0, nil, nil, NSStringFromClass([AppDelegate class]));
    }
    
}

#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    printf("FINISHED LAUNCHING");
    // Override point for customization after application launch.

    // Create a window
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    // Create a view controller
    UIViewController *viewController = [[UIViewController alloc] init];

    viewController.view.backgroundColor = UIColor.redColor;

    // Set the view controller as the root view controller
    self.window.rootViewController = viewController;

    // Make the window visible
    [self.window makeKeyAndVisible];

    return YES;
}

@end

int main(int argc, char * argv[]) {
    @autoreleasepool {
        printf("MAIN");
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#include "Native/Application/nApplication.h"
#include <cstdio>
#include <stdexcept>

#import <Cocoa/Cocoa.h>

static uApplication* app;

@interface AppDelegate : NSResponder <NSApplicationDelegate>

@property NSMenu* appMenu;

@end

@implementation AppDelegate 

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    
    _appMenu = [[NSMenu alloc] initWithTitle:@"My App Menu"];
    
    // Create a "File" menu item
    NSMenuItem *fileMenuItem = [[NSMenuItem alloc] initWithTitle:@"File" action:nil keyEquivalent:@""];
    NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    [fileMenuItem setSubmenu:fileMenu];
    
    // Add "Quit" item to the "File" menu
    NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
    [fileMenu addItem:quitMenuItem];
    
    NSMenuItem *quitMenuItem2 = [[NSMenuItem alloc] initWithTitle:@"Quit 2" action:@selector(terminate:) keyEquivalent:@"q"];
    [fileMenu addItem:quitMenuItem2];
    
    // Add the "File" menu to the main menu
    [_appMenu addItem:fileMenuItem];
    
    // Create more menu items as needed
    
    // Set the main menu
    [NSApp setMainMenu:_appMenu];


    if (app == nullptr) {
        printf("[CRITICAL] Invalid uApplication instance!\n");
        exit(EXIT_FAILURE);
    }
    
  
    app->FinishedLaunching();
    
    if (app->simpleWindow != nullptr) {
        //app->simpleWindow->OnCreated();
        return;
    }

    
}

@end

int nApplication::Run(uApplication* application) {
    app = application;
    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];
        
        AppDelegate* appDelegate = [[AppDelegate alloc] init];
        [app setDelegate: appDelegate];
        
        [NSApp run];
        
    }
    
    return 0;
    
}

use universalui_types::{ application::uApplication, ffi::* };
use universalui_types::*;


use ffi::*;

/// This is the internal 'main' application function for universalui
/// and where all control ultimately originates from. 
pub fn universalui_main(app: uApplication) -> i32 {

    println!("Starting UniversalUI Main...");
    println!("App title: {}", ffi_cchar_to_str(app.title));
    println!("App developer: {}", ffi_cchar_to_str(app.developer));
    println!("App version: {}.{}.{}", app.version.major, app.version.minor, app.version.build);

    if !universalui_native::native::init() {
        return -1;
    }

    // call app launched callback if it exists
    match app.launched_callback {
        Some(launched_callback) => (launched_callback)(),
        None =>  print_warning!("No launched_callback set for app '{}'", ffi_cchar_to_str(app.title))
    }

    println!("starting loop...");

    // run native events loop
    universalui_native::native::run();

    return 0;
}



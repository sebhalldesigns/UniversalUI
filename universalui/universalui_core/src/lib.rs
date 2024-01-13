use universalui_types::application::uApplication;

pub mod ffi;

use ffi::*;

/// This is the internal 'main' application function for universalui
/// and where all control ultimately originates from. 
pub fn universalui_main(app: uApplication) -> i32 {

    println!("Starting UniversalUI Main...");
    println!("App title: {}", ffi_cchar_to_str(app.title));
    println!("App developer: {}", ffi_cchar_to_str(app.developer));
    println!("App version: {}.{}.{}", app.version.major, app.version.minor, app.version.build);
    
    (app.launched_callback)();


    return 0;
}



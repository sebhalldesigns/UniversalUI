
use universalui_types::application::uApplication;
use universalui_core::*;

#[no_mangle]
pub extern "C" fn UniversalUI(app: uApplication) -> libc::c_int {
    return universalui_main(app);
}

#[no_mangle]
pub extern "C" fn CreateWindow(title: *const libc::c_char, width: libc::c_int, height: libc::c_int) -> libc::c_int {
    println!("hello from rust!");
    return 0;
}
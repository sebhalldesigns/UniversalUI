use std::{rc::Rc, cell::*, ptr::{null, null_mut}};

use universalui_types::{ application::*,  ui::*, layout::*, ffi::*};
use universalui_core::*;

#[no_mangle]
pub extern "C" fn UniversalUI(app: uApplication) -> libc::c_int {
    return universalui_main(app);
}

#[no_mangle]
pub extern "C" fn CreateWindow(title: *mut libc::c_char, width: libc::c_float, height: libc::c_float) -> *mut uWindow {

    let window: Option<*mut uWindow> = universalui_native::native::native_try_allocate_window(title, uSize { width: width, height: height});

    match window {
        Some(window) => {
             // Get the raw pointer from the Box
            let raw_ptr = window.clone();

            // Print the memory address
            unsafe {
                println!("Memory Address: {} {}", raw_ptr.clone() as u64, (&((*window).will_resize) as *const _)  as u64);
            }
            
            return raw_ptr;
        }
        None => {
            return null_mut();
        }
    }
}
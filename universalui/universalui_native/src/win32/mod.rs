use self::win32_base::{win32_create_window, win32_run};
use std::ffi::{c_void, c_char};
use universalui_types::{*, layout::*, ui::*, ffi::*};
use std::{rc::Rc, cell::*};

mod win32_base;

pub fn init() -> bool {
    return win32_base::win32_init();
}

pub fn native_try_allocate_window(title: *mut c_char, size: uSize) -> Option<*mut uWindow> {
    return win32_create_window(title, size);
}

pub fn run() {
    return win32_run();
}
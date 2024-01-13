#![allow(non_camel_case_types)]

use crate::layout::*;
use std::ffi::{c_void, c_char};

#[repr(C)]
#[derive(Clone, Copy)]
pub struct uWindow {
    pub raw_handle: *mut c_void,
    pub title: *mut c_char,
    pub size: uSize,

    pub will_resize: uWindowWillResizeCallback
}


//  FUNCTIONS

/// Called when the window will be resized
type uWindowWillResizeCallback = Option<extern "C" fn(size: uSize)>;


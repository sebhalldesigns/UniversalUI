use crate::layout::*;
use std::ffi::CString;

#[repr(C)]
struct uWindow {
    title: CString,
    size: uSize
}

struct UView {

}

use std::ffi::{CStr, c_char};


/// Safely convert a C string to a Rust string
pub fn ffi_cchar_to_str(string: *const c_char) -> String {
    if string.is_null() {
        return String::from("[STRING ERROR]"); 
    }

    unsafe {
        let c_str = CStr::from_ptr(string);
        match c_str.to_str() {
            Ok(str_slice) => {
                return String::from(str_slice);
            },
            Err(_) => {
                return String::from("[STRING ERROR]"); 
            }
        }
    }
}
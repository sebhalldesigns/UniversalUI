#[macro_use]
extern crate lazy_static;

#[cfg_attr(windows, path = "win32/mod.rs")]
pub mod native;

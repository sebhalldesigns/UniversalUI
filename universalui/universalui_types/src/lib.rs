
pub mod layout;

pub mod ui;

pub mod application;
pub mod info;
pub mod ffi;

pub mod graphics;

#[macro_export]
macro_rules! print_info {
    ($prefix:expr) => {
        println!("\x1b[34m[INFO]\x1b[0m {}", $prefix);
    };
    ($prefix:expr, $($arg:tt)*) => {
        println!("\x1b[34m[INFO]\x1b[0m {}", format_args!($prefix, $($arg)*));
    };
}


#[macro_export]
macro_rules! print_warning {
    ($prefix:expr) => {
        println!("\x1b[33m[WARNING]\x1b[0m {}", $prefix);
    };
    ($prefix:expr, $($arg:tt)*) => {
        println!("\x1b[33m[WARNING]\x1b[0m {}", format_args!($prefix, $($arg)*));
    };    
}


#[macro_export]
macro_rules! print_error {

    ($prefix:expr) => {
        println!("\x1b[31m[ERROR]\x1b[0m {}", $prefix);
    };
    ($prefix:expr, $($arg:tt)*) => {
        println!("\x1b[31m[ERROR]\x1b[0m {}", format_args!($prefix, $($arg)*));
    };
    
}

#[macro_export]
macro_rules! print_critical {

    ($prefix:expr) => {
        println!("\x1b[91m[CRITICAL]\x1b[0m {}", $prefix);
    };
    ($prefix:expr, $($arg:tt)*) => {
        println!("\x1b[91m[CRITICAL]\x1b[0m {}", format_args!($prefix, $($arg)*));
    };
    
}






pub mod layout;

pub mod ui;

pub mod application;
pub mod info;
pub mod ffi;

pub mod graphics;

use std::collections::*;
use std::collections::hash_map::Entry;
use std::sync::*;
use ui::*;

#[derive(Clone)]
/// The global container of UniversalUI data
pub struct UniversalUI {
    windows: HashMap<isize, Arc<RwLock<uWindow>>>,
}

impl UniversalUI {

    pub fn new() -> Self {

        return UniversalUI {
            windows: HashMap::new()
        };
    }

    pub fn TrySetWindow(mut self, key: isize, window: uWindow) -> Option<Arc<RwLock<uWindow>>> {

        match self.windows.entry(key) {
            Entry::Occupied(_) => {
                // window already exists for this key
                return None;
            },
            Entry::Vacant(entry) => {
                let allocated = Arc::new(RwLock::new(window));
                entry.insert(allocated.clone());
                return Some(allocated);
            }
        }

    }

    pub fn TryGetWindow(mut self, key: isize) -> Option<Arc<RwLock<uWindow>>> {
        match self.windows.entry(key) {
            Entry::Occupied(entry) => {
                // window exists for this key
                return Some(entry.get().clone());
            },
            Entry::Vacant(_) => {
                // window does not exist for this key
                return None;
            }
        }
    }

    pub fn RemoveWindow(mut self, key: isize) {
        self.windows.remove(&key);
    }
}


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





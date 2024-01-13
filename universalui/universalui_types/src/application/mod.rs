#![allow(non_camel_case_types)]

use std::os::raw::c_char;

use crate::info::uVersion;



// TYPES

#[repr(C)]
/// Defines a UniversalUI application, containing basic information
/// about the app, and providing the top-level interface 
/// into UniversalUI.
pub struct uApplication {
    pub title: *const c_char,
    pub developer: *const c_char,
    pub version: uVersion,

    pub launched_callback: uApplicationLaunchedCallback,
    pub should_quit_callback: uApplicationShouldQuitCallback,
    pub will_quit_callback: uApplicationWillQuitCallback
}

//  FUNCTIONS

/// Called when universalui has launched successfully
type uApplicationLaunchedCallback = extern "C" fn();

/// Called when the system has detected an optional quit trigger
/// this includes things like Cmd+Q on a mac or Ctrl+Q on other platforms
type uApplicationShouldQuitCallback = extern "C" fn() -> bool;

/// Called when the system is quitting without an option. 
/// This function should not take long to execute as it may have a 
/// time limit (e.g on iOS).
type uApplicationWillQuitCallback = extern "C" fn();




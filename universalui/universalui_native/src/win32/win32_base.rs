use windows::{
    core::*,
    Win32::{
        Foundation::*,
        UI::WindowsAndMessaging::*,
        System::LibraryLoader::*, Graphics::Gdi::*,
    },
};

use std::{rc::Rc, cell::{RefCell, Ref, UnsafeCell}, sync::Mutex, iter::Map, collections::HashMap, borrow::{BorrowMut, Borrow}, ptr::{null, null_mut}, mem::size_of};
use universalui_types::{*, layout::*, ui::*, ffi::*, graphics::{uPixelBuffer, uPixel}};
use std::ffi::{c_void, c_char};
use std::sync::*;
use std::iter::*;
use std::collections::hash_map::Entry;

// Create a newtype wrapper for HWND

const WIN32_WINDOW_CLASS_NAME: PCWSTR = w!("UNIVERSALUI_WINDOW");

thread_local! {
    static WINDOWS: Mutex<HashMap<isize, *mut uWindow>> = {
        let mut map = HashMap::new();
        Mutex::new(map)
    };
}




pub fn win32_init() -> bool {

    // local function to get instance handle
    fn get_instance() -> Result<HINSTANCE> {
        unsafe {
            let instance = GetModuleHandleW(None)?.into();
            return Ok(instance);
        }
    }

    // local function to register window class
    fn register_class(instance: HINSTANCE) -> Result<bool> {
        unsafe {
            
            let window_class = WNDCLASSW {
                hCursor: LoadCursorW(None, IDC_ARROW)?,
                hInstance: instance,
                lpszClassName: WIN32_WINDOW_CLASS_NAME,
                style: CS_HREDRAW | CS_VREDRAW,
                lpfnWndProc: Some(win32_window_procedure),
                ..Default::default()
            };

            let atom = RegisterClassW(&window_class);

            if atom == 0 {
                print_critical!("failed to register Win32 window class!");
                return Ok(false);
            } else {
                return Ok(true);
            }
        }
    }

    let instance = match get_instance() {
        Ok(inst) => inst,
        Err(_) => {
            print_critical!("failed to get Win32 Module Handle"); 
            return false; 
        }
    };

    match register_class(instance) {
        Ok(result) => return result,
        Err(_) => return false
    };
}

pub fn win32_create_window(title: *mut c_char, size: uSize) -> Option<*mut uWindow> {

   

    // local function to get instance handle
    fn get_instance() -> Result<HINSTANCE> {
        unsafe {
            let instance = GetModuleHandleW(None)?.into();
            return Ok(instance);
        }
    }

    fn create_window(title: String, size: uSize, instance: HINSTANCE) -> Result<HWND> {
        unsafe {
            let mut encoded = title
                .encode_utf16()
                .chain([0u16])
                .collect::<Vec<u16>>();

             let widened = HSTRING::from(title);
            let pcwstr = widened.as_wide().as_ptr();


            return Ok(CreateWindowExW(
                WINDOW_EX_STYLE::default(),
                WIN32_WINDOW_CLASS_NAME,
                PCWSTR { 0: pcwstr } ,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                size.width as i32,
                size.height as i32,
                None,
                None,
                instance,
                None,
            ));
        }
    }

    let instance: HINSTANCE = match get_instance() {
        Ok(inst) => inst,
        Err(_) => {
            print_critical!("failed to get Win32 Module Handle"); 
            return None; 
        }
    };

    let window_handle: HWND = match create_window(ffi_cchar_to_str(title), size, instance) {
        Ok(handle) => handle,
        Err(_) => {
            print_critical!("failed to create a Win32 window"); 
            return None; 
        }
    };

    unsafe {
        ShowWindow(window_handle, SW_SHOW);
    }

    let window = uWindow {
        raw_handle: window_handle.0.clone() as *mut c_void,
        title: title,
        size: size,
        will_resize: None
    };
    
    let window_box: Box<uWindow> = Box::new(
        
            window
        
    );

    // Get the raw pointer from the Box
    let raw_ptr = Box::into_raw(window_box);

    // Print the memory address
    println!("Memory Address: {}", raw_ptr as u64);

    // Access the thread-local variable
    return WINDOWS.with(|windows| {
        
        // Insert or update an entry using the `entry` method
        match windows.lock().unwrap().entry(window_handle.0.clone()) {
            Entry::Occupied(_) => {
                print_error!("window with a duplicate handle created!");
                return None;
            }, 
            Entry::Vacant(entry) => {
                // new window handle, so insert and return
                entry.insert(raw_ptr.clone());
                print_info!("inserted window! {}", window_handle.0.clone());
                return Some(raw_ptr);
            }
        }
    });

}

pub fn win32_run() {

   

    // Main message loop
    let mut msg: MSG = Default::default();

    loop {
        unsafe {
            // Peek and dispatch messages
            while PeekMessageW(&mut msg, HWND(0), 0, 0, PM_REMOVE).0 != 0 {
                println!(" loop...");
                
                if msg.message == WM_QUIT {
                    // Exit the loop when WM_QUIT is received
                    return;
                }

                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            if WINDOWS.with(|windows| {
                // Insert or update an entry using the `entry` method
                return windows.lock().unwrap().len() == 0;

            }) {
                return;
            }

        }
    }
}

unsafe extern "system" fn win32_window_procedure(hwnd: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {  
    
    unsafe fn LOWORD(x: u32) -> u16 {
        (x & 0xFFFF) as u16
    }

    unsafe fn HIWORD(x: u32) -> u16 {
        ((x >> 16) & 0xFFFF) as u16
    }


    fn try_get_window(matching: HWND) -> Option<*mut uWindow> {
         // Access the thread-local variable
         return WINDOWS.with(|windows| {
            
            // Insert or update an entry using the `entry` method
            match windows.lock().unwrap().entry(matching.0.clone()) {
                Entry::Occupied(window) => {
                    //print_info!("window found {}", window.get().clone() as u64);

                    return Some(window.get().clone());
                }, 
                Entry::Vacant(_) => {
                    // new window handle, so insert and return
                    //print_error!("no window for {}", matching.0.clone());
                    return None;
                }
            }
        });
    }

    let window: Option<*mut uWindow> = try_get_window(hwnd);

    if window.is_none() {
        //println!("no window found");
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }

    match message {
        WM_SIZE => {

            let width = LOWORD(lparam.0 as u32);
            let height = HIWORD(lparam.0 as u32);

            let new_size: uSize = uSize { width: width as f32, height: height as f32};
            let window_ptr = window.unwrap();

            // call window resize if it exists
            match (*window_ptr).will_resize {
                Some(will_resize) => (will_resize)(new_size.clone()),
                None =>  print_warning!("No will_resize set for window '{}'", ffi_cchar_to_str((*window_ptr).title))
            }

            (*window_ptr).size = new_size; 

        }

        WM_DESTROY => {
            WINDOWS.with(|windows| {
                // Insert or update an entry using the `entry` method
                windows.lock().unwrap().remove(&hwnd.0.clone());
            });
            
            std::alloc::dealloc(window.unwrap() as *mut u8, std::alloc::Layout::new::<uWindow>());
            // make it safe to deallocate

        }

        WM_PAINT => {

            let size: uSize = (*window.unwrap()).size.clone();
            let width: i32 = size.width as i32;
            let height: i32 = size.height as i32;
            
            let mut ps = PAINTSTRUCT::default();
            let hdc_window = BeginPaint(hwnd, &mut ps);

            let mut buffer: uPixelBuffer = uPixelBuffer {
                width: size.width,
                height: size.height,
                pixels: Vec::new()
            };

            buffer.pixels.resize_with((buffer.width*buffer.height) as usize, || { uPixel { red: 255, green: 0, blue: 255, alpha: 255 } });

            for pixel in buffer.pixels.iter_mut() {
                pixel.red = 255;
                pixel.alpha = 255;
                //println!("{} {} {} {}", pixel.red, pixel.green, pixel.blue, pixel.alpha);
            }

            // Create a bitmap from the buffer
            let hbitmap = CreateBitmap(width, height, 1, 32, Some(buffer.pixels.as_ptr() as *const c_void));

            // Create a compatible DC and select the bitmap into it
            let hdc_memory = CreateCompatibleDC(hdc_window);
            let old_bitmap = SelectObject(hdc_memory, hbitmap);

            // Copy the bitmap to the window's DC
            BitBlt(hdc_window, 0, 0, width, height, hdc_memory, 0, 0, SRCCOPY);

            // Cleanup
            SelectObject(hdc_memory, old_bitmap);
            DeleteObject(hbitmap);
            DeleteDC(hdc_memory);
            EndPaint(hwnd, &ps);

            /* 
             // Create a compatible DC
            let hdc_memory = CreateCompatibleDC(hdc_window);
            let rect = ps.rcPaint;

            // Define BITMAPINFO structure for the DIB
            let mut bi = BITMAPINFO {
                bmiHeader: BITMAPINFOHEADER {
                    biSize: std::mem::size_of::<BITMAPINFOHEADER>() as u32,
                    biWidth: width,
                    biHeight: -height,// negative height to create a top-down DIB
                    biPlanes: 1,
                    biBitCount: 32, // RGBA 32-bit
                    biCompression: BI_RGB.0,
                    ..Default::default()
                },
                ..Default::default()
            };

             // Create a DIB Section
            let mut bits = std::ptr::null_mut();
            let hbitmap: HBITMAP = CreateDIBSection(hdc_memory, &bi, DIB_RGB_COLORS, &mut bits, HWND {0: 0}, 0).unwrap();

            // Obtain a pointer to the pixel buffer
            let buffer = std::slice::from_raw_parts_mut(bits as *mut u32, (width * height) as usize);

            // Modify the pixel buffer (RGBA 32-bit format)
            for y in 0..height {
                for x in 0..width {
                    let offset = (y * width + x) as usize;
                    // Set pixel data (ARGB format, alpha set to 255)
                    buffer[offset] = 0xFF00FF00; // example: green color
                }
            }

             // Select the bitmap into the DC and perform the drawing
            let old_bitmap = SelectObject(hdc_memory, hbitmap);
            BitBlt(hdc_window, 0, 0, width, height, hdc_memory, 0, 0, SRCCOPY);
            SelectObject(hdc_memory, old_bitmap);

            // Cleanup
            DeleteObject(hbitmap);
            DeleteDC(hdc_memory);
            EndPaint(hwnd, &ps);
            */
            /* 

            let bmi_header: BITMAPINFOHEADER = BITMAPINFOHEADER {
                biSize: std::mem::size_of::<BITMAPINFOHEADER>() as u32,
                biWidth: width,
                biHeight: -height,
                biPlanes: 1,
                biBitCount: 32,
                biCompression: BI_RGB.0,
                ..Default::default()
            };


            let bitmapInfo: BITMAPINFO = BITMAPINFO {
                bmiHeader: bmi_header,
                bmiColors: std::mem::zeroed()
            };

            let mut buffer: uPixelBuffer = uPixelBuffer {
                width: size.width,
                height: size.height,
                pixels: Vec::new()
            };

            buffer.pixels.resize_with((buffer.width*buffer.height) as usize, || { uPixel { red: 255, green: 255, blue: 255, alpha: 255 } });

            for pixel in buffer.pixels.iter_mut() {
                pixel.red = 255;
                pixel.alpha = 255;
                //println!("{} {} {} {}", pixel.red, pixel.green, pixel.blue, pixel.alpha);
            }

            let buffer_ptr: *mut uPixel = &mut buffer.pixels[0];

            let h_bitmap = CreateDIBSection(mem_dc, &bitmapInfo, DIB_RGB_COLORS, &mut (buffer_ptr as *mut c_void), HWND {0: 0}, 0);

            // Step 3: Select the DIB into the compatible DC
            let h_old_bitmap = SelectObject(mem_dc, h_bitmap.unwrap());

            // Step 4: Manipulate the framebuffer (modify 'bits' directly)

            // Step 5: Present the framebuffer to the window
            
            _ = BitBlt(hwnd_dc, 0, 0, size.width as i32, size.height as i32, mem_dc, 0, 0, SRCCOPY);
            

            // Step 6: Clean up resources
            SelectObject(mem_dc, h_old_bitmap);
            DeleteDC(mem_dc);
            ReleaseDC(hwnd, hwnd_dc);

            */

        }
        _ => {

        }
    }
    
    
    return DefWindowProcW(hwnd, message, wparam, lparam);
}
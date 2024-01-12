use windows::{
    core::*,
    Win32::{
        Foundation::*,
        UI::WindowsAndMessaging::*,
        System::LibraryLoader::*,
    },
};

fn main() -> Result<()> {
    unsafe {

        fn get_instance() -> Result<HMODULE> {
            unsafe {
                let instance = GetModuleHandleW(None)?;
                return Ok(instance);
            }
        }
    
        let instance: Option<HMODULE> = match get_instance() {
            Ok(inst) => Some(inst),
            Err(_) => {
                //debug_critical("couldn't get instance handle!"); 
                None
            }
        };
    
        let window_class = w!("window");
        let window_name = "Rust Win32 Window";

        let wc = WNDCLASSW {
            hCursor: LoadCursorW(None, IDC_ARROW).unwrap(),
            hInstance: instance.unwrap().into(),
            lpszClassName: window_class,

            style: CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc: Some(wndproc),
            ..Default::default()
        };

        RegisterClassW(&wc);

         let hwnd: HWND = CreateWindowExW(
            WINDOW_EX_STYLE::default(),
            window_class,
            window_class,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            None,
            None,
            instance.unwrap(),
            None,
        );
        ShowWindow(hwnd, SW_SHOW);

 
        if hwnd.0 == 0 {
            return Err(Error::from_win32());
        }

        let mut message = MSG::default();

        while GetMessageW(&mut message, HWND(0), 0, 0).into() {
            DispatchMessageW(&message);
        }

        Ok(())
    }
}


unsafe extern "system" fn wndproc(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {  
    return DefWindowProcA(window, message, wparam, lparam);
}
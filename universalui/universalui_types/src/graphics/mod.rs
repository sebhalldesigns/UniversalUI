

pub struct uPixel {
    pub red: u8,
    pub green: u8,
    pub blue: u8,
    pub alpha: u8
}

pub struct uPixelBuffer {
    pub width: f32,
    pub height: f32,
    pub pixels: Vec<uPixel>
}
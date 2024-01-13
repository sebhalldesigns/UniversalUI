

pub struct uPixel {
    pub red: f32,
    pub green: f32,
    pub blue: f32,
    pub alpha: f32
}

pub struct uPixelBuffer {
    pub width: f32,
    pub height: f32,
    pub pixels: Vec<uPixel>
}

#[derive(Default)]
#[derive(Clone)]
#[derive(Copy)]
#[repr(C)]
pub struct uSize {
    pub width: f32,
    pub height: f32
}

#[derive(Default)]
#[derive(Clone)]
#[derive(Copy)]
pub struct uColor {
    r: f32,
    g: f32,
    b: f32,
    a: f32
}
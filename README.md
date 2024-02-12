# UniversalUI
a lightweight application development framework

## Technologies

### Abstraction Layers

- **System** - an abstraction of windowing and I/O (mouse, keyboard etc) functions.
- **Graphics** - an abstraction over different graphics APIs: OpenGL, Vulkan & Metal.
 
### Rendering
- **ACR** (Abstract Component Representation) - an abstraction of rendering components into a platform, hardware and renderer independant format. 
- **WTI** (Web Technology Integrator) - provides translation of ACR units into HTML and CSS documents.
- **CCR** (Core Component Renderer) - a lightweight cross-platform 2D rendering system for non web platforms.
- **GP3R** (General Purpose 3D Renderer) - a cross-platform 3D renderer targeting high flexibility and compatibility.
### Other
- **RML** (Reduced Markup Language) - an XML-style markup language with a reduced syntax for faster and simpler parsing.


## Notes

- CSR should be a pixel buffer for the selected 

## Removed

### Rendering

UniversalUI contains three different rendering systems which can be combined with each other.
- **CSR** (Core Software Renderer) - a lightweight cross-platform software renderer focusing on compatibility and simplicity. Runs on any device regardless of GPU and graphics support and relies purely on low-level windowing APIs (Win32, X11 etc).
- **AHR** (Accelerated Hardware Renderer) - a cross-platform GPU renderer that focuses on performance and reducing CPU load.
- **GP3R** (General Purpose 3D Renderer) - a cross-platform 3D renderer targeting high flexibility and compatibility. 
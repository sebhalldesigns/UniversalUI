# UniversalUI
UniversalUI - a high-performance cross-platform UI framework

Notes on architecture:
SDL2 very ideal for the project - reliable windowing across all major platforms including WASM, efficient 2D abstraction layer
ImGui also seems like a good component - has SDL2 renderer backend and can render fonts and vector graphics.

ImGui doesn't really offer the functionality we want so here is the plan:
- UniversalUI defines the C++ interface and layout system.
- Imgui used for rendering.

For now will ditch attempts at a retained-mode GUI for simplicity.
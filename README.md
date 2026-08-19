# 🕐 3D-Clock — Laser-Cut Wood Themed Animated Clock

A real-time animated **3D analog clock** built with OpenGL/freeglut in C++, styled to look like a laser-cut wooden clock — pale birch wood for the face/body and dark burnt-wood accents (simulating laser-cut edges). The scene features an orbiting camera you can drag around with the mouse.

## Features

- 🪵 **Laser-cut wood material theme** — light birch wood (`setBirchWood`) for the main body/face and dark burnt-wood shading (`setBurntWood`) for cut edges, using proper OpenGL ambient/diffuse/specular lighting materials
- 🕐 Animated clock hands/elements driven by an `animFrame` timer for smooth motion
- 🎥 **Interactive orbiting camera** — drag with the mouse to rotate; controlled via `camYaw`, `camPitch`, and `camRadius`
- 💡 Realistic lighting via `glMaterialfv` (ambient, diffuse, specular) and shininess settings for a matte wood look
- 🎞️ Continuous render loop built on GLUT's animation/display callbacks

## Requirements

- A C++ compiler with **OpenGL** and **freeglut** support (this project was built in Visual Studio 2022)
- `GL/freeglut.h` available on the include path (install via NuGet — e.g. `freeglut` package — or link the library manually)

## Building & Running

1. Open `Clock.sln` (or the project) in Visual Studio 2022.
2. Make sure the freeglut package/library is installed and referenced (see **External Dependencies** in Solution Explorer).
3. Build and run (`Local Windows Debugger`).
4. Click and drag with the mouse to orbit the camera around the clock.

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Application entry point — camera controls, wood materials, and clock rendering/animation logic |

## Controls

| Action | Result |
|---|---|
| Left-click + drag | Orbit the camera (adjusts `camYaw` / `camPitch`) |
| *(scroll/zoom, if implemented)* | Adjusts `camRadius` |

## License

Feel free to use and adapt this for coursework or personal learning.

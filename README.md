# Minecraft OpenGL Renderer

A 3D Minecraft-inspired block renderer built with OpenGL, featuring lighting, textures, and camera controls.

## Features

- 3D block rendering with multiple cube objects
- Texture mapping (brick, dirt, redstone lamp textures)
- Dynamic lighting system with ambient and diffuse lighting
- First-person camera with keyboard and mouse controls
- Material system with specular highlighting
- Random block placement and rotation animations

## Screenshots

The project renders textured cubes in a 3D environment with lighting effects and camera movement.

## Dependencies

- **OpenGL** - Graphics rendering
- **GLFW** - Window management and input handling
- **GLM** - OpenGL Mathematics library for vectors and matrices
- **GLAD** - OpenGL loader (included)
- **stb_image** - Image loading for textures (included)

## Build Instructions

### macOS (using Homebrew)

1. Install dependencies:
```bash
brew install glfw glm pkg-config
```

2. Clone the repository:
```bash
git clone <your-repo-url>
cd Minecraft
```

3. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

4. Run the executable:
```bash
./Minecraft
```

### Alternative Build (CLion/CMake)

1. Open the project in CLion or any CMake-compatible IDE
2. Configure the project (CMake should auto-detect dependencies)
3. Build and run

## Controls

- **W/A/S/D** - Move camera forward/left/backward/right
- **Space** - Move camera up
- **Left Shift** - Move camera down
- **Mouse** - Look around
- **ESC** - Exit application

## Project Structure

```
├── src/           # Source files
├── include/       # Header files
├── Shaders/       # GLSL shader files
├── Textures/      # Texture assets
├── glad/          # OpenGL loader
├── stb/           # Image loading library
└── CMakeLists.txt # Build configuration
```

## Technical Details

- Written in C++17
- Uses modern OpenGL with vertex arrays and shaders
- Implements Phong lighting model
- Supports texture mapping with UV coordinates
- Camera system with perspective projection

## License

This project is open source. Feel free to use and modify as needed. 
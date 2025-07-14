# Flappy Bird MVP - SDL + Emscripten

A simple MVP (Minimum Viable Product) implementation of Flappy Bird using SDL2 and Emscripten for web deployment.

## Features

- **Simple Physics**: Gravity and jump mechanics
- **Colored Graphics**: Uses colored rectangles for all game elements
- **Space to Jump**: Press SPACE to make the bird jump
- **No Obstacles**: This is a simple MVP focused on basic mechanics
- **Web-Ready**: Compiles to WebAssembly using Emscripten

## Game Elements

- **Bird**: Yellow rectangle with black eye and orange beak
- **Background**: Sky blue background
- **Ground**: Green rectangle at the bottom
- **Physics**: Gravity pulls the bird down, SPACE makes it jump

## Controls

- `SPACE` - Make the bird jump

## Building

### Prerequisites

- Emscripten SDK (emsdk) installed and activated
- CMake
- Ninja build system

### Build Steps

1. Navigate to the project directory
2. Run the build script:
   ```bash
   build_and_run.bat
   ```

Or manually:
```bash
cd build
ninja
```

### Running

Open `build/index.html` in a web browser to play the game.

## Project Structure

```
Demo-SDL/
├── src/
│   └── main.cpp          # Main game code
├── build/
│   ├── DemoSDL.js        # Generated JavaScript
│   ├── DemoSDL.wasm      # Generated WebAssembly
│   └── index.html        # Game HTML page
├── CMakeLists.txt        # Build configuration
└── build_and_run.bat     # Build script
```

## Technical Details

- **Language**: C++20
- **Graphics**: SDL2 (via Emscripten port)
- **Build System**: CMake + Ninja
- **Target**: WebAssembly (WASM)
- **Frame Rate**: 60 FPS

## Game Mechanics

1. **Gravity**: Constant downward acceleration
2. **Jump**: Instant upward velocity when SPACE is pressed
3. **Boundaries**: Bird cannot go above screen top or below ground
4. **Continuous Loop**: Game runs at 60 FPS using emscripten_set_main_loop

## Future Enhancements

- Add obstacles (pipes)
- Add scoring system
- Add sound effects
- Add particle effects
- Add game over screen
- Add texture assets

[Application::Run()]
 ├── Poll SDL Events
 ├── SystemManager.UpdateAll()
 │    ├── MovementSystem
 │    ├── AnimationSystem
 │    ├── CameraSystem              ← Moves camera entities (cinematic, follow, etc.)
 │    ├── SpriteRenderSystem        ← Pushes RenderCommands to GameplayLayer
 │    └── UISystem                  ← Pushes RenderCommands to UILayer
 │
 ├── Renderer.BeginFrame()         ← SDL_RenderClear()
 │
 ├── For each [CameraEntity] with (CameraComponent + TransformComponent)
 │    ├── if (camera.active)
 │    │
 │    ├── SDL_RenderSetViewport(camera.viewport)
 │    │
 │    ├── For each Layer in LayerStack
 │    │    ├── if (layer.enabled)
 │    │    │
 │    │    ├── For each RenderCommand in layer.RenderQueue
 │    │    │    ├── Apply camera scroll (dst.x -= camera.x, dst.y -= camera.y)
 │    │    │    ├── Optionally apply camera.zoom (scale dst rect)
 │    │    │    └── SDL_RenderCopy(renderer, texture, &src, &adjusted_dst)
 │    │    │
 │    │    └── (optional: sort RenderCommands by zOrder before drawing)
 │    │
 │    └── SDL_RenderSetViewport(nullptr) ← Reset to full screen
 │
 ├── Renderer.RenderUILayers()     ← Draws UI layers without camera offset
 │    ├── (or treat UI as a fixed camera with viewport = full screen)
 │
 └── Renderer.EndFrame()           ← SDL_RenderPresent()

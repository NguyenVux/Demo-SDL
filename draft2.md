# EnTT Integration Guide for Demo-SDL Project

## Step 1: Adding EnTT to Project

### Option A: Using vcpkg (Recommended)

```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# Install EnTT
./vcpkg install entt
```

### Option B: Git Submodule

```bash
# In your project root
git submodule add https://github.com/skypjack/entt.git external/entt
git submodule update --init --recursive
```

### Option C: Manual Download

1. Download EnTT from: https://github.com/skypjack/entt
2. Extract to `external/entt/` folder in your project

## Step 2: Update CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)
project(DemoSDL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SDL2 REQUIRED)

# Option A: If using vcpkg
find_package(EnTT CONFIG REQUIRED)

# Option B: If using submodule or manual
# add_subdirectory(external/entt)

# Create executable
add_executable(DemoSDL
    src/main.cpp
    src/pch.h
    src/Application.cpp
    src/Application.h
    src/AssetsManager.cpp
    src/AssetsManager.h
    src/Animation.cpp
    src/Animation.h
    src/BaseChar.cpp
    src/BaseChar.h
    src/DebugLayer.cpp
    src/DebugLayer.h
    src/FSM.cpp
    src/FSM.h
    src/ILayer.h
    src/IState.h
    src/LayerStack.cpp
    src/LayerStack.h
    src/MainGameLayer.cpp
    src/MainGameLayer.h
    src/RenderQueue.cpp
    src/RenderQueue.h
    src/Time.cpp
    src/Time.h

    # New ECS files
    src/ECSWorld.cpp
    src/ECSWorld.h
    src/Components.h
    src/Systems.cpp
    src/Systems.h
    src/IInputHandler.h
    src/PlayerInputHandler.cpp
    src/PlayerInputHandler.h
    src/AIInputHandler.cpp
    src/AIInputHandler.h
    src/CharacterStates.cpp
    src/CharacterStates.h
    src/CharacterFactory.cpp
    src/CharacterFactory.h
    src/CharacterManager.cpp
    src/CharacterManager.h
)

# Link libraries
target_link_libraries(DemoSDL 
    SDL2::SDL2
    SDL2::SDL2main
)

# Option A: If using vcpkg
target_link_libraries(DemoSDL EnTT::EnTT)

# Option B: If using submodule or manual
# target_link_libraries(DemoSDL EnTT)

# Include directories
target_include_directories(DemoSDL PRIVATE src/)

# Option B: If using submodule or manual
# target_include_directories(DemoSDL PRIVATE external/entt/src)

# Precompiled headers
target_precompile_headers(DemoSDL PRIVATE src/pch.h)
```

## Step 3: Update pch.h

```cpp
#pragma once

// Standard library
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cassert>

// SDL2
#include <SDL.h>

// EnTT - Add this for performance
#include <entt/entt.hpp>

// Project headers
#include "Time.h"
```

## Step 4: Create ECSWorld.h

```cpp
// src/ECSWorld.h
#pragma once
#include "pch.h"

class RenderQueue;

// Entity type alias for clarity
using Entity = entt::entity;

// Option 1: Static Approach (Simple, Global Access)
class ECSWorld {
public:
    static void Initialize();
    static void Cleanup();
    static entt::registry& GetRegistry() { return s_registry; }

    // Utility functions
    static Entity CreateEntity();
    static void DestroyEntity(Entity entity);
    static bool IsValidEntity(Entity entity);

    // Debug functions
    static size_t GetEntityCount();
    static void PrintEntityInfo(Entity entity);

private:
    static entt::registry s_registry;
};

// Option 2: Instance Approach (Better for Testing, Multiple Worlds)
class ECSWorldInstance {
public:
    ECSWorldInstance();
    ~ECSWorldInstance();

    void Initialize();
    void Cleanup();
    entt::registry& GetRegistry() { return m_registry; }

    // Utility functions
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsValidEntity(Entity entity);

    // Debug functions
    size_t GetEntityCount();
    void PrintEntityInfo(Entity entity);

private:
    entt::registry m_registry;
};

// Option 3: Singleton Approach (Global Access + Instance Benefits)
class ECSWorldSingleton {
public:
    static ECSWorldSingleton& GetInstance();

    void Initialize();
    void Cleanup();
    entt::registry& GetRegistry() { return m_registry; }

    // Utility functions
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsValidEntity(Entity entity);

    // Debug functions
    size_t GetEntityCount();
    void PrintEntityInfo(Entity entity);

private:
    ECSWorldSingleton() = default;
    ~ECSWorldSingleton() = default;
    ECSWorldSingleton(const ECSWorldSingleton&) = delete;
    ECSWorldSingleton& operator=(const ECSWorldSingleton&) = delete;

    entt::registry m_registry;
};
```

## Step 5: Create ECSWorld.cpp

```cpp
// src/ECSWorld.cpp
#include "pch.h"
#include "ECSWorld.h"

// ==============================================
// Option 1: Static Implementation
// ==============================================

// Static member definition
entt::registry ECSWorld::s_registry;

void ECSWorld::Initialize() {
    // Initialize any global ECS resources
    s_registry.ctx().emplace<entt::dispatcher>();
    std::cout << "ECS World (Static) initialized" << std::endl;
}

void ECSWorld::Cleanup() {
    s_registry.clear();
    std::cout << "ECS World (Static) cleaned up" << std::endl;
}

Entity ECSWorld::CreateEntity() {
    return s_registry.create();
}

void ECSWorld::DestroyEntity(Entity entity) {
    if (IsValidEntity(entity)) {
        s_registry.destroy(entity);
    }
}

bool ECSWorld::IsValidEntity(Entity entity) {
    return s_registry.valid(entity);
}

size_t ECSWorld::GetEntityCount() {
    return s_registry.alive();
}

void ECSWorld::PrintEntityInfo(Entity entity) {
    if (!IsValidEntity(entity)) {
        std::cout << "Invalid entity: " << static_cast<uint32_t>(entity) << std::endl;
        return;
    }

    std::cout << "Entity " << static_cast<uint32_t>(entity) << " components: ";
    bool hasComponents = false;

    // You need to include Components.h or forward declare the components
    // if (s_registry.all_of<Transform>(entity)) {
    //     std::cout << "Transform ";
    //     hasComponents = true;
    // }

    if (!hasComponents) {
        std::cout << "None";
    }

    std::cout << std::endl;
}

// ==============================================
// Option 2: Instance Implementation
// ==============================================

ECSWorldInstance::ECSWorldInstance() {
    Initialize();
}

ECSWorldInstance::~ECSWorldInstance() {
    Cleanup();
}

void ECSWorldInstance::Initialize() {
    m_registry.ctx().emplace<entt::dispatcher>();
    std::cout << "ECS World (Instance) initialized" << std::endl;
}

void ECSWorldInstance::Cleanup() {
    m_registry.clear();
    std::cout << "ECS World (Instance) cleaned up" << std::endl;
}

Entity ECSWorldInstance::CreateEntity() {
    return m_registry.create();
}

void ECSWorldInstance::DestroyEntity(Entity entity) {
    if (IsValidEntity(entity)) {
        m_registry.destroy(entity);
    }
}

bool ECSWorldInstance::IsValidEntity(Entity entity) {
    return m_registry.valid(entity);
}

size_t ECSWorldInstance::GetEntityCount() {
    return m_registry.alive();
}

void ECSWorldInstance::PrintEntityInfo(Entity entity) {
    if (!IsValidEntity(entity)) {
        std::cout << "Invalid entity: " << static_cast<uint32_t>(entity) << std::endl;
        return;
    }

    std::cout << "Entity " << static_cast<uint32_t>(entity) << " components: ";
    // Component checks would go here
    std::cout << "None" << std::endl;
}

// ==============================================
// Option 3: Singleton Implementation
// ==============================================

ECSWorldSingleton& ECSWorldSingleton::GetInstance() {
    static ECSWorldSingleton instance;
    return instance;
}

void ECSWorldSingleton::Initialize() {
    m_registry.ctx().emplace<entt::dispatcher>();
    std::cout << "ECS World (Singleton) initialized" << std::endl;
}

void ECSWorldSingleton::Cleanup() {
    m_registry.clear();
    std::cout << "ECS World (Singleton) cleaned up" << std::endl;
}

Entity ECSWorldSingleton::CreateEntity() {
    return m_registry.create();
}

void ECSWorldSingleton::DestroyEntity(Entity entity) {
    if (IsValidEntity(entity)) {
        m_registry.destroy(entity);
    }
}

bool ECSWorldSingleton::IsValidEntity(Entity entity) {
    return m_registry.valid(entity);
}

size_t ECSWorldSingleton::GetEntityCount() {
    return m_registry.alive();
}

void ECSWorldSingleton::PrintEntityInfo(Entity entity) {
    if (!IsValidEntity(entity)) {
        std::cout << "Invalid entity: " << static_cast<uint32_t>(entity) << std::endl;
        return;
    }

    std::cout << "Entity " << static_cast<uint32_t>(entity) << " components: ";
    // Component checks would go here
    std::cout << "None" << std::endl;
}
```

## Step 6: Create Components.h

```cpp
// src/Components.h
#pragma once
#include "pch.h"
#include <string>
#include <unordered_map>
#include <memory>

// Forward declarations
class AnimationInstance;
class IInputHandler;

// Transform Component
struct Transform {
    SDL_FPoint position{0.0f, 0.0f};
    SDL_FPoint velocity{0.0f, 0.0f};
    SDL_FPoint scale{1.0f, 1.0f};
    float rotation = 0.0f;
    bool facingRight = true;
};

// RigidBody Component
struct RigidBody {
    float mass = 1.0f;
    float drag = 0.0f;
    bool useGravity = true;
    bool isKinematic = false;
    float gravityScale = 1.0f;

    void ApplyForce(const SDL_FPoint& force) {
        // Force = mass * acceleration, so acceleration = force / mass
        // This would be applied in the physics system
    }

    void ApplyImpulse(const SDL_FPoint& impulse) {
        // Impulse directly changes velocity
        // This would be applied in the physics system
    }
};

// Collider Component (for future collision system)
enum class ColliderType {
    BOX,
    CIRCLE,
    CAPSULE
};

struct Collider {
    ColliderType type = ColliderType::BOX;
    SDL_FPoint size{32.0f, 32.0f};
    SDL_FPoint offset{0.0f, 0.0f};
    bool isTrigger = false;
    uint32_t layer = 0;
    uint32_t mask = 0xFFFFFFFF;
};

// Input State for character input
struct InputState {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool attack = false;
    bool crouch = false;
};

// Input Component
struct Input {
    std::unique_ptr<IInputHandler> inputHandler;
    InputState currentState;
    InputState previousState;

    void Update() {
        if (inputHandler != nullptr) {
            previousState = currentState;
            currentState = inputHandler->GetInputState();
        }
    }
};

// Animation Types (Fixed set for all characters)
enum class AnimationType : uint8_t {
    IDLE = 0,
    WALKING,
    RUNNING,
    JUMPING,
    FALLING,
    ATTACKING,
    TAKING_DAMAGE,
    CROUCHING,
    COUNT
};

// Animator Component - Simplified Playback State Only
// Animation data is stored globally and referenced by type
struct Animator {
    AnimationType currentAnimation = AnimationType::IDLE;
    AnimationType previousAnimation = AnimationType::IDLE;
    
    // Current playback state (resets to 0 when animation changes)
    float currentTime = 0.0f;
    int currentFrame = 0;
    bool isPlaying = true;
    bool hasFinished = false;
    
    // Animation properties for current animation
    float frameRate = 12.0f;    // Frames per second
    int totalFrames = 1;        // Total frames in current animation
    bool isLooping = true;      // Whether current animation loops
    
    void PlayAnimation(AnimationType type) {
        if (currentAnimation != type) {
            previousAnimation = currentAnimation;
            currentAnimation = type;
            
            // Reset playback state to beginning
            currentTime = 0.0f;
            currentFrame = 0;
            isPlaying = true;
            hasFinished = false;
            
            // Animation properties would be set by AnimationSystem
            // based on the animation type
        }
    }
    
    void Stop() {
        isPlaying = false;
    }
    
    void Resume() {
        if (!hasFinished) {
            isPlaying = true;
        }
    }
    
    bool HasAnimationChanged() const {
        return currentAnimation != previousAnimation;
    }
};

## Animation Architecture - Stateless Component Approach

### Design Philosophy:
Your chosen approach separates **animation data** (stored globally) from **playback state** (stored in component). This is ideal for ECS because:

1. **Small Component Size**: Only playback state, no heavy animation data
2. **Cache Friendly**: Components are tiny and contiguous
3. **Resource Sharing**: Multiple entities can share the same animation data
4. **State Reset**: Every animation switch starts from frame 0
5. **Predictable Memory**: Fixed component size regardless of animation complexity

### How It Works:

```cpp
// Global Animation Manager (Singleton or Static)
class AnimationManager {
public:
    static const AnimationInstance* GetAnimation(AnimationType type);
    static float GetFrameRate(AnimationType type);
    static int GetTotalFrames(AnimationType type);
    static bool IsLooping(AnimationType type);
    
private:
    static std::array<std::unique_ptr<AnimationInstance>, static_cast<size_t>(AnimationType::COUNT)> s_animations;
    // Animation metadata
    static std::array<float, static_cast<size_t>(AnimationType::COUNT)> s_frameRates;
    static std::array<int, static_cast<size_t>(AnimationType::COUNT)> s_totalFrames;
    static std::array<bool, static_cast<size_t>(AnimationType::COUNT)> s_isLooping;
};

// Animation System Updates All Animators
class AnimationSystem {
public:
    static void Update(ECSWorldInstance& world) {
        auto& registry = world.GetRegistry();
        auto view = registry.view<Animator>();
        
        for (auto entity : view) {
            auto& animator = view.get<Animator>(entity);
            
            if (!animator.isPlaying) continue;
            
            // Get animation metadata from global manager
            animator.frameRate = AnimationManager::GetFrameRate(animator.currentAnimation);
            animator.totalFrames = AnimationManager::GetTotalFrames(animator.currentAnimation);
            animator.isLooping = AnimationManager::IsLooping(animator.currentAnimation);
            
            // Update playback time
            animator.currentTime += Time::GetDeltaTimeF();
            
            // Calculate current frame
            float frameTime = 1.0f / animator.frameRate;
            animator.currentFrame = static_cast<int>(animator.currentTime / frameTime);
            
            // Handle animation end
            if (animator.currentFrame >= animator.totalFrames) {
                if (animator.isLooping) {
                    animator.currentTime = 0.0f;
                    animator.currentFrame = 0;
                } else {
                    animator.currentFrame = animator.totalFrames - 1;
                    animator.hasFinished = true;
                    animator.isPlaying = false;
                }
            }
        }
    }
    
    static void Render(ECSWorldInstance& world, RenderQueue& queue) {
        auto& registry = world.GetRegistry();
        auto view = registry.view<Transform, Animator>();
        
        for (auto entity : view) {
            auto& transform = view.get<Transform>(entity);
            auto& animator = view.get<Animator>(entity);
            
            // Get animation data from global manager
            const AnimationInstance* animation = AnimationManager::GetAnimation(animator.currentAnimation);
            if (animation != nullptr) {
                // Render current frame
                animation->RenderFrame(animator.currentFrame, transform.position, queue);
            }
        }
    }
};
```

### Usage Example:

```cpp
// Create character entity
Entity player = world.CreateEntity();
auto& animator = world.GetRegistry().emplace<Animator>(player);

// Switch animations - always starts from frame 0
animator.PlayAnimation(AnimationType::WALKING);  // Starts at frame 0
animator.PlayAnimation(AnimationType::JUMPING);  // Starts at frame 0
animator.PlayAnimation(AnimationType::IDLE);     // Starts at frame 0

// Check current state
if (animator.currentAnimation == AnimationType::JUMPING && animator.hasFinished) {
    animator.PlayAnimation(AnimationType::FALLING);
}
```

### Benefits of This Approach:

- ✅ **Ultra-small components** (64 bytes vs potentially KB with embedded animations)
- ✅ **Perfect cache performance** when processing many entities
- ✅ **No pointer chasing** during ECS iteration
- ✅ **Deterministic behavior** (always starts from frame 0)
- ✅ **Easy state machine integration** (clear animation transitions)
- ✅ **Memory efficient** (shared animation data)

// Character Controller Component
struct CharacterController {
    // Movement parameters
    float moveSpeed = 200.0f;
    float jumpForce = 444.0f;
    float acceleration = 1000.0f;
    float deceleration = 800.0f;

    // State flags
    bool isGrounded = false;
    bool canMove = true;
    bool canJump = true;
};

// Health Component
struct Health {
    float maxHealth = 100.0f;
    float currentHealth = 100.0f;
    bool isDead = false;
    float invulnerabilityTime = 0.0f;

    bool IsAlive() const { return currentHealth > 0.0f && !isDead; }
    bool CanTakeDamage() const { return invulnerabilityTime <= 0.0f; }
};

// State Machine Component
struct StateMachine {
    FSM fsm;

    void AddState(int stateId, std::unique_ptr<IState> state) {
        fsm.AddState(stateId, std::move(state));
    }

    void ChangeState(int stateId) {
        fsm.RequestStateChange(stateId);
    }

    int GetCurrentStateId() const {
        return fsm.GetCurrentStateID();
    }
};
```

## ECS World Architecture Comparison

### Option 1: Static Approach

**Pros:**

- Simple to use - just call `ECSWorld::CreateEntity()`
- No need to pass registry around
- Fast access (no pointer dereferencing)
- Traditional game engine approach

**Cons:**

- Global state (harder to test)
- Cannot have multiple worlds
- Harder to reset/reinitialize
- Static initialization order issues

**Usage Example:**

```cpp
// Usage
ECSWorld::Initialize();
Entity player = ECSWorld::CreateEntity();
auto& transform = ECSWorld::GetRegistry().emplace<Transform>(player);
```

### Option 2: Instance Approach

**Pros:**

- Better for unit testing
- Can have multiple ECS worlds
- Clear ownership and lifetime
- No global state issues
- Easy to reset/reinitialize

**Cons:**

- Need to pass world instance around
- Slightly more complex API
- Small performance overhead

**Usage Example:**

```cpp
// Usage
ECSWorldInstance world;
Entity player = world.CreateEntity();
auto& transform = world.GetRegistry().emplace<Transform>(player);

// In systems, you'd pass the world reference
void UpdatePhysics(ECSWorldInstance& world) {
    auto view = world.GetRegistry().view<Transform, RigidBody>();
    // ...
}
```

### Option 3: Singleton Approach

**Pros:**

- Global access like static
- Instance benefits for testing
- Thread-safe initialization
- Can still have multiple instances for testing

**Cons:**

- Still global state
- Singleton complexity
- Slightly more verbose access

**Usage Example:**

```cpp
// Usage
ECSWorldSingleton::GetInstance().Initialize();
Entity player = ECSWorldSingleton::GetInstance().CreateEntity();
auto& transform = ECSWorldSingleton::GetInstance().GetRegistry().emplace<Transform>(player);
```

## Recommended Approach for Your Project

For a game like yours, I recommend **Option 2 (Instance)** because:

1. **Better Architecture**: Clear ownership in Application class
2. **Testing**: Easier to unit test systems
3. **Flexibility**: Can create temporary worlds for testing
4. **Modern C++**: Follows RAII principles

Here's how to integrate it:

## Step 7: Update Application.h (Instance Approach)

```cpp
// src/Application.h
#pragma once
#include "pch.h"
#include "ECSWorld.h"

class LayerStack;
class AssetsManager;
class BaseChar;

class Application {
public:
    Application();
    ~Application();

    void Run();

    // Provide access to ECS world for other classes
    ECSWorldInstance& GetECSWorld() { return m_ecsWorld; }

private:
    void Initialize();
    void Cleanup();
    void HandleEvents();
    void Update();
    void Render();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_running;

    std::unique_ptr<LayerStack> m_layerStack;
    std::unique_ptr<AssetsManager> m_assetManager;

    // ECS World instance
    ECSWorldInstance m_ecsWorld;
    Entity m_playerEntity;

    // Old system (to be phased out)
    std::unique_ptr<BaseChar> m_char;

    // Helper functions
    void CreateTestEntity();
    void UpdateECSSystems();
    void TestEnTTBasics();
};
```

```cpp
// src/Application.h
#pragma once
#include "pch.h"
#include "ECSWorld.h" // Add this
#include "CharacterManager.h" // Add this when created

class LayerStack;
class AssetsManager;
class BaseChar;

class Application {
public:
    Application();
    ~Application();

    void Run();

private:
    void Initialize();
    void Cleanup();
    void HandleEvents();
    void Update();
    void Render();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_running;

    std::unique_ptr<LayerStack> m_layerStack;
    std::unique_ptr<AssetsManager> m_assetManager;

    // Old system (to be phased out)
    std::unique_ptr<BaseChar> m_char;

    // New ECS system
    std::unique_ptr<CharacterManager> m_characterManager; // Add this when CharacterManager is created
    Entity m_playerEntity; // Add this
};
```

## Step 8: Update Application.cpp (Instance Approach)

```cpp
// src/Application.cpp
#include "pch.h"
#include "Application.h"
#include "Components.h"
// ... other includes

Application::Application() 
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_running(false)
    , m_playerEntity(entt::null)
{
    Initialize();
}

Application::~Application() {
    Cleanup();
}

void Application::Initialize() {
    // ... existing SDL initialization code ...

    // ECS World is automatically initialized in constructor
    // But you can call Initialize() if you need custom setup
    m_ecsWorld.Initialize();

    // Create a simple test entity
    CreateTestEntity();

    // Test EnTT basics
    TestEnTTBasics();

    // ... rest of initialization ...
}

void Application::Cleanup() {
    // ... existing cleanup code ...

    // ECS World automatically cleaned up in destructor
    // But you can call Cleanup() explicitly if needed
    m_ecsWorld.Cleanup();
}

void Application::CreateTestEntity() {
    // Create a simple entity to test EnTT integration
    Entity testEntity = m_ecsWorld.CreateEntity();

    // Add Transform component
    auto& transform = m_ecsWorld.GetRegistry().emplace<Transform>(testEntity);
    transform.position = {100.0f, 100.0f};
    transform.scale = {2.0f, 2.0f};

    // Add RigidBody component
    auto& rigidBody = m_ecsWorld.GetRegistry().emplace<RigidBody>(testEntity);
    rigidBody.useGravity = true;
    rigidBody.mass = 1.0f;

    // Add CharacterController
    auto& controller = m_ecsWorld.GetRegistry().emplace<CharacterController>(testEntity);
    controller.moveSpeed = 200.0f;

    std::cout << "Created test entity with ID: " << static_cast<uint32_t>(testEntity) << std::endl;
    m_ecsWorld.PrintEntityInfo(testEntity);

    m_playerEntity = testEntity;
}

void Application::Update() {
    // ... existing update code ...

    // Update ECS systems
    UpdateECSSystems();

    // ... rest of update code ...
}

void Application::UpdateECSSystems() {
    // Simple physics system example
    auto& registry = m_ecsWorld.GetRegistry();
    auto view = registry.view<Transform, RigidBody>();

    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        auto& rigidBody = view.get<RigidBody>(entity);

        if (rigidBody.useGravity) {
            constexpr float gravity = -9.8f * 44.4f;
            transform.velocity.y += gravity * rigidBody.gravityScale * Time::GetDeltaTimeF();
        }

        // Update position
        transform.position.x += transform.velocity.x * Time::GetDeltaTimeF();
        transform.position.y += transform.velocity.y * Time::GetDeltaTimeF();

        // Simple ground collision (temporary)
        if (transform.position.y <= 0.0f) {
            transform.position.y = 0.0f;
            transform.velocity.y = 0.0f;
        }
    }
}

void Application::TestEnTTBasics() {
    std::cout << "=== EnTT Integration Test ===" << std::endl;

    auto& registry = m_ecsWorld.GetRegistry();

    // Test 1: Create entities
    Entity entity1 = m_ecsWorld.CreateEntity();
    Entity entity2 = m_ecsWorld.CreateEntity();

    std::cout << "Created entities: " << static_cast<uint32_t>(entity1) 
              << ", " << static_cast<uint32_t>(entity2) << std::endl;

    // Test 2: Add components
    registry.emplace<Transform>(entity1);
    registry.emplace<RigidBody>(entity1);

    registry.emplace<Transform>(entity2);
    // entity2 doesn't have RigidBody

    // Test 3: Check components
    std::cout << "Entity1 has Transform: " << registry.all_of<Transform>(entity1) << std::endl;
    std::cout << "Entity1 has RigidBody: " << registry.all_of<RigidBody>(entity1) << std::endl;
    std::cout << "Entity2 has Transform: " << registry.all_of<Transform>(entity2) << std::endl;
    std::cout << "Entity2 has RigidBody: " << registry.all_of<RigidBody>(entity2) << std::endl;

    // Test 4: View iteration
    auto view = registry.view<Transform, RigidBody>();
    std::cout << "Entities with both Transform and RigidBody: ";
    for (auto entity : view) {
        std::cout << static_cast<uint32_t>(entity) << " ";
    }
    std::cout << std::endl;

    // Test 5: Component access
    auto& transform1 = registry.get<Transform>(entity1);
    transform1.position = {50.0f, 75.0f};
    std::cout << "Entity1 position: (" << transform1.position.x 
              << ", " << transform1.position.y << ")" << std::endl;

    // Test 6: Entity count
    std::cout << "Total entities: " << m_ecsWorld.GetEntityCount() << std::endl;

    std::cout << "=== EnTT Test Complete ===" << std::endl;
}
```

## Step 9: Systems with Instance Approach

When using the instance approach, your systems would receive the ECS world as a parameter:

```cpp
// src/Systems.h
#pragma once
#include "ECSWorld.h"

class RenderQueue;

// Physics System
class PhysicsSystem {
public:
    static void Update(ECSWorldInstance& world);

private:
    static void ApplyGravity(ECSWorldInstance& world);
    static void UpdateVelocity(ECSWorldInstance& world);
};

// Input System  
class InputSystem {
public:
    static void Update(ECSWorldInstance& world);
};

// Animation System
class AnimationSystem {
public:
    static void Update(ECSWorldInstance& world);
    static void Render(ECSWorldInstance& world, RenderQueue& queue);
};

// Example implementation:
/*
void PhysicsSystem::Update(ECSWorldInstance& world) {
    auto& registry = world.GetRegistry();
    auto view = registry.view<Transform, RigidBody>();

    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        auto& rigidBody = view.get<RigidBody>(entity);

        // Physics logic here
    }
}
*/
```

## Alternative: Systems as Classes

If you prefer, you can make systems as classes that hold a reference to the world:

```cpp
// Alternative approach - Systems as classes
class PhysicsSystem {
public:
    PhysicsSystem(ECSWorldInstance& world) : m_world(world) {}

    void Update();

private:
    ECSWorldInstance& m_world;

    void ApplyGravity();
    void UpdateVelocity();
};

// Usage in Application:
class Application {
private:
    ECSWorldInstance m_ecsWorld;
    PhysicsSystem m_physicsSystem;
    InputSystem m_inputSystem;
    // ...

public:
    Application() : m_physicsSystem(m_ecsWorld), m_inputSystem(m_ecsWorld) {}

    void Update() {
        m_inputSystem.Update();
        m_physicsSystem.Update();
        // ...
    }
};
```

## Step 10: Build and Test

1. Build your project:
   
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

2. Run the executable and check console output for EnTT test results.

## Expected Output

When you run the application, you should see output like:

```
ECS World initialized
Created test entity with ID: 0
Entity 0 components: Transform 
=== EnTT Integration Test ===
Created entities: 1, 2
Entity1 has Transform: 1
Entity1 has RigidBody: 1
Entity2 has Transform: 1
Entity2 has RigidBody: 0
Entities with both Transform and RigidBody: 0 1 
Entity1 position: (50, 75)
Total entities: 3
=== EnTT Test Complete ===
```

## Next Steps

After confirming EnTT integration works:

1. Implement the Systems classes (PhysicsSystem, InputSystem, etc.)
2. Create CharacterFactory and CharacterManager
3. Gradually migrate from BaseChar to the new ECS system
4. Add more components as needed
5. Implement collision detection system

This setup gives you a solid foundation for the ECS refactoring while maintaining your existing codebase during the transition.

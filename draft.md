# BaseChar Refactoring Draft

## Current Issues Analysis

### Problems with Current Implementation:

1. **Tight Coupling**: BaseChar directly handles input, physics, and rendering logic
2. **State Management**: Using enum-based states instead of proper FSM implementation
3. **Hard-coded Behavior**: Movement and physics logic is directly in Update() method
4. **Non-extensible**: Difficult to create different character types (Player vs NPC)
5. **Mixed Responsibilities**: Character class handles rendering, input, physics, and animation
6. **Input Coupling**: Direct SDL keyboard handling in character class
7. **No Component System**: Monolithic character class

## Proposed Refactoring Architecture

### 1. Entity-Component-System (ECS) Approach with EnTT

#### ECS Setup with EnTT

```cpp
// ECSWorld.h
#pragma once
#include "pch.h"
#include <entt/entt.hpp>

class RenderQueue;

// Global ECS registry - can be made into a singleton or passed around
extern entt::registry g_registry;

// Entity is just an entt::entity typedef
using Entity = entt::entity;
```

#### Component System

```cpp
// Components.h
#pragma once
#include "pch.h"
#include <string>
#include <unordered_map>
#include <memory>

class AnimationInstance;

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

    void ApplyForce(const SDL_FPoint& force);
    void ApplyImpulse(const SDL_FPoint& impulse);
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

// Animator Component - manages multiple animations for different states
struct Animator {
    std::unordered_map<std::string, std::unique_ptr<AnimationInstance>> animations;
    std::string currentAnimation;
    std::string previousAnimation;
    bool animationChanged = false;

    void PlayAnimation(const std::string& name);
    void StopAnimation();
    AnimationInstance* GetCurrentAnimation();
    bool HasAnimation(const std::string& name) const;
};

// Character Controller Component
struct CharacterController {
    // Movement parameters
    float moveSpeed = 200.0f;
    float jumpForce = 444.0f; // 10.0f * 44.4f from current code
    float acceleration = 1000.0f;
    float deceleration = 800.0f;

    // State flags
    bool isGrounded = false;
    bool canMove = true;
    bool canJump = true;
};

// Input Component - stores current input state for the entity
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

// Health Component (example of another dependency)
struct Health {
    float maxHealth = 100.0f;
    float currentHealth = 100.0f;
    bool isDead = false;
    float invulnerabilityTime = 0.0f;

    bool IsAlive() const { return currentHealth > 0.0f && !isDead; }
    bool CanTakeDamage() const { return invulnerabilityTime <= 0.0f; }
};
```

### 2. ECS Systems with EnTT

#### Input Handler Interface

```cpp
// IInputHandler.h
#pragma once
#include "pch.h"

struct InputState {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool attack = false;
    bool crouch = false;
    // Add more inputs as needed
};

class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual InputState GetInputState() = 0;
    virtual void Update() {}
};

// PlayerInputHandler.h
#pragma once
#include "IInputHandler.h"

class PlayerInputHandler : public IInputHandler {
public:
    InputState GetInputState() override;
    void Update() override;
};

// AIInputHandler.h (for NPCs)
#pragma once
#include "IInputHandler.h"
#include "ECSWorld.h"

class AIInputHandler : public IInputHandler {
public:
    AIInputHandler(Entity target = entt::null);
    InputState GetInputState() override;
    void Update() override;

    void SetTarget(Entity target) { m_target = target; }
    void SetBehavior(/* AI behavior parameters */);

private:
    Entity m_target;
    // AI state and decision making variables
};
```

#### ECS Systems

```cpp
// Systems.h
#pragma once
#include "ECSWorld.h"

class RenderQueue;

// Physics System
class PhysicsSystem {
public:
    static void Update();
    static void ApplyGravity();
    static void UpdateVelocity();
};

// Input System  
class InputSystem {
public:
    static void Update();
};

// Animation System - handles component dependencies
class AnimationSystem {
public:
    static void Update();
    static void Render(RenderQueue& queue);

private:
    // Helper to sync animations with state machine
    static void SyncAnimationWithState(Entity entity);
    static void HandleAnimationTransitions(Entity entity);
};

// Character Controller System - depends on Input, Transform, RigidBody
class CharacterControllerSystem {
public:
    static void Update();

private:
    static void HandleMovement(Entity entity);
    static void HandleJump(Entity entity);
    static void HandleActions(Entity entity);

    // Helper methods that work with multiple components
    static bool IsEntityGrounded(Entity entity);
    static void UpdateGroundedStatus(Entity entity);
};

// State Machine System - coordinates with Animator
class StateMachineSystem {
public:
    static void Update();

private:
    // Handle state transitions based on multiple component states
    static void CheckStateTransitions(Entity entity);
    static void UpdateAnimationForState(Entity entity, int newState);
};

// Render System
class RenderSystem {
public:
    static void Render(RenderQueue& queue);
    static void RenderDebug(RenderQueue& queue);
};

// Health System (example of another system with dependencies)
class HealthSystem {
public:
    static void Update();

private:
    static void ProcessDamage(Entity entity);
    static void HandleDeath(Entity entity);
};
```

### 3. State Machine Integration with EnTT

#### Character States Using FSM

```cpp
// CharacterStates.h
#pragma once
#include "IState.h"
#include "ECSWorld.h"

class CharacterState : public IState {
protected:
    CharacterState(Entity owner, FSM* fsm);

public:
    Entity GetOwner() { return m_owner; }

protected:
    Entity m_owner;

    // Helper methods using EnTT
    Transform* GetTransform();
    RigidBody* GetRigidBody();
    Animator* GetAnimator();
    CharacterController* GetController();
};

enum class CharacterStateType : int {
    IDLE = 0,
    MOVING = 1,
    JUMPING = 2,
    FALLING = 3,
    ATTACKING = 4,
    TAKING_DAMAGE = 5,
    CROUCHING = 6,
    WALL_SLIDING = 7,
    DASHING = 8
};

class IdleState : public CharacterState {
public:
    IdleState(Entity owner, FSM* fsm) : CharacterState(owner, fsm) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

class MovingState : public CharacterState {
public:
    MovingState(Entity owner, FSM* fsm) : CharacterState(owner, fsm) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

class JumpingState : public CharacterState {
public:
    JumpingState(Entity owner, FSM* fsm) : CharacterState(owner, fsm) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

class FallingState : public CharacterState {
public:
    FallingState(Entity owner, FSM* fsm) : CharacterState(owner, fsm) {}

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// Add more states as needed...
```

#### State Machine Component

```cpp
// StateMachine Component
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

### 4. Character Factory System with EnTT

```cpp
// CharacterFactory.h
#pragma once
#include "ECSWorld.h"

enum class CharacterType {
    PLAYER,
    NPC_GUARD,
    NPC_CIVILIAN,
    ENEMY_BASIC,
    ENEMY_BOSS
};

class CharacterFactory {
public:
    static Entity CreateCharacter(CharacterType type);

private:
    static Entity CreatePlayer();
    static Entity CreateNPC(/* NPC parameters */);
    static Entity CreateEnemy(/* Enemy parameters */);
};

// Example implementation:
/*
Entity CharacterFactory::CreatePlayer() {
    Entity entity = g_registry.create();

    // Add components using EnTT
    g_registry.emplace<Transform>(entity);
    g_registry.emplace<RigidBody>(entity);
    g_registry.emplace<Animator>(entity);
    g_registry.emplace<CharacterController>(entity);
    g_registry.emplace<StateMachine>(entity);

    // Set up player-specific properties
    auto& transform = g_registry.get<Transform>(entity);
    transform.position = {0.0f, 200.0f};
    transform.scale = {2.0f, 2.0f};

    auto& controller = g_registry.get<CharacterController>(entity);
    controller.moveSpeed = 200.0f;
    controller.jumpForce = 444.0f;

    // Set up state machine
    auto& stateMachine = g_registry.get<StateMachine>(entity);
    stateMachine.AddState(static_cast<int>(CharacterStateType::IDLE), 
                         std::make_unique<IdleState>(entity, &stateMachine.fsm));
    stateMachine.AddState(static_cast<int>(CharacterStateType::MOVING), 
                         std::make_unique<MovingState>(entity, &stateMachine.fsm));
    // ... add more states

    return entity;
}
*/
```

### 5. Character Manager System with EnTT

```cpp
// CharacterManager.h
#pragma once
#include "ECSWorld.h"
#include <vector>

class RenderQueue;

class CharacterManager {
public:
    CharacterManager();
    ~CharacterManager();

    Entity CreateCharacter(CharacterType type);
    void RemoveCharacter(Entity entity);

    void UpdateAll();
    void RenderAll(RenderQueue& queue);
    void RenderDebugAll(RenderQueue& queue);

    // For collision system integration
    std::vector<Entity> GetCharactersInArea(const SDL_FRect& area);
    std::vector<Entity> GetAllCharacters();

private:
    // EnTT handles entity storage, we just manage the systems
    void InitializeSystems();
};

// Example system update calls:
/*
void CharacterManager::UpdateAll() {
    InputSystem::Update();
    CharacterControllerSystem::Update();
    PhysicsSystem::Update();
    StateMachineSystem::Update();
    AnimationSystem::Update();
}

void CharacterManager::RenderAll(RenderQueue& queue) {
    AnimationSystem::Render(queue);
    RenderSystem::Render(queue);
}
*/
```

## Implementation Plan

### Phase 1: EnTT Setup and Core Infrastructure

1. Add EnTT library to project (vcpkg, git submodule, or manual)
2. Create ECSWorld.h with global registry
3. Define basic components (Transform, RigidBody, Collider)
4. Update CMakeLists.txt to include EnTT

### Phase 2: Systems Implementation

1. Implement core systems (PhysicsSystem, InputSystem)
2. Create IInputHandler interface and implementations
3. Implement CharacterControllerSystem
4. Update FSM to work with EnTT entities

### Phase 3: Animation and Rendering Systems

1. Refactor Animator component for EnTT
2. Implement AnimationSystem and RenderSystem
3. Integrate with existing Animation/AnimationInstance system

### Phase 4: Character Types and Factory

1. Implement CharacterFactory with EnTT entity creation
2. Create different character presets (Player, NPCs, Enemies)
3. Implement CharacterManager with system orchestration
4. Update Application to use new ECS system

### Phase 5: Collision Preparation

1. Finalize Collider component structure
2. Prepare collision event system using EnTT signals/events
3. Design collision layer/mask system
4. Create collision detection system interfaces (for future implementation)

## Benefits of Using EnTT

1. **Performance**: EnTT is one of the fastest ECS libraries available
2. **Memory Efficiency**: Components are stored in contiguous memory (SoA)
3. **Flexibility**: Mix and match components freely
4. **Type Safety**: Compile-time type checking
5. **Modern C++**: Uses C++17 features, header-only library
6. **Active Development**: Well-maintained and documented
7. **Entity Relationships**: Support for entity hierarchies and relationships
8. **Events/Signals**: Built-in event system for loose coupling

## Component Dependency Best Practices

### 1. Minimize Hard Dependencies

- Use optional components when possible (`g_registry.all_of<ComponentType>(entity)`)
- Design components to be as independent as possible
- Use events/signals for loose coupling between systems

### 2. Manager Components Pattern

```cpp
// Good: Animator manages multiple animations based on external state
struct Animator {
    std::unordered_map<std::string, std::unique_ptr<AnimationInstance>> animations;
    std::string currentAnimation;
    // ... methods to manage animations
};

// Systems coordinate between components
void AnimationSystem::Update() {
    auto view = g_registry.view<Animator, StateMachine>();
    for (auto entity : view) {
        // Sync animation with state machine
        SyncAnimationWithState(entity);
    }
}
```

### 3. Composition Over Inheritance

```cpp
// Instead of: class PlayerCharacter : public BaseCharacter
// Use: Entity with Player-specific component combination

Entity player = g_registry.create();
g_registry.emplace<Transform>(player);
g_registry.emplace<RigidBody>(player);
g_registry.emplace<Input>(player);          // Player-specific
g_registry.emplace<Inventory>(player);      // Player-specific
g_registry.emplace<PlayerController>(player); // Player-specific behavior
```

### 4. System Ordering for Dependencies

```cpp
// Update systems in correct order to handle dependencies
void CharacterManager::UpdateAll() {
    // 1. Input first
    InputSystem::Update();

    // 2. Game logic that depends on input
    CharacterControllerSystem::Update();
    StateMachineSystem::Update();

    // 3. Physics after game logic
    PhysicsSystem::Update();

    // 4. Animation after state changes
    AnimationSystem::Update();

    // 5. Effects and other systems
    HealthSystem::Update();
}
```

### 5. Event-Driven Dependencies

```cpp
// Use EnTT's event system for loose coupling
struct StateChangedEvent {
    Entity entity;
    int oldState;
    int newState;
};

// In StateMachineSystem
void StateMachineSystem::ChangeState(Entity entity, int newState) {
    auto& stateMachine = g_registry.get<StateMachine>(entity);
    int oldState = stateMachine.GetCurrentStateId();

    stateMachine.ChangeState(newState);

    // Emit event instead of directly calling animation system
    auto& dispatcher = g_registry.ctx().get<entt::dispatcher>();
    dispatcher.trigger<StateChangedEvent>(entity, oldState, newState);
}

// In AnimationSystem
void AnimationSystem::OnStateChanged(const StateChangedEvent& event) {
    if (g_registry.all_of<Animator>(event.entity)) {
        UpdateAnimationForState(event.entity, event.newState);
    }
}
```

## EnTT Integration Examples

### Component Dependencies and Patterns

#### 1. Manager Components with State Dependencies

```cpp
// Example: Animator component that manages multiple animations based on state
void AnimationSystem::Update() {
    // Process entities that have both Animator and StateMachine
    auto view = g_registry.view<Animator, StateMachine>();

    for (auto entity : view) {
        auto& animator = view.get<Animator>(entity);
        auto& stateMachine = view.get<StateMachine>(entity);

        // Get current state name for animation lookup
        std::string stateName = GetStateNameFromId(stateMachine.GetCurrentStateId());

        // Check if we need to change animation
        if (animator.currentAnimation != stateName) {
            if (animator.HasAnimation(stateName)) {
                animator.previousAnimation = animator.currentAnimation;
                animator.currentAnimation = stateName;
                animator.animationChanged = true;
                animator.PlayAnimation(stateName);
            }
        }

        // Update current animation
        if (auto* anim = animator.GetCurrentAnimation()) {
            anim->Update();
        }
    }
}
```

#### 2. Multi-Component System Dependencies

```cpp
// Example: Character controller that depends on Input, Transform, RigidBody, and Health
void CharacterControllerSystem::Update() {
    // Process entities with all required components
    auto view = g_registry.view<CharacterController, Transform, RigidBody, Input>();

    for (auto entity : view) {
        auto& controller = view.get<CharacterController>(entity);
        auto& transform = view.get<Transform>(entity);
        auto& rigidBody = view.get<RigidBody>(entity);
        auto& input = view.get<Input>(entity);

        // Optional component dependency - check if entity has Health
        Health* health = nullptr;
        if (g_registry.all_of<Health>(entity)) {
            health = &g_registry.get<Health>(entity);
        }

        // Skip if character is dead
        if (health && !health->IsAlive()) {
            return;
        }

        // Update grounded status based on physics
        UpdateGroundedStatus(entity);

        // Handle movement based on input
        if (controller.canMove) {
            HandleMovement(entity);
        }

        // Handle jumping
        if (controller.canJump && controller.isGrounded) {
            HandleJump(entity);
        }
    }
}
```

#### 3. Component Cross-Communication Patterns

```cpp
// Example: State machine that coordinates with multiple components
void StateMachineSystem::Update() {
    auto view = g_registry.view<StateMachine, Transform, CharacterController>();

    for (auto entity : view) {
        auto& stateMachine = view.get<StateMachine>(entity);
        auto& transform = view.get<Transform>(entity);
        auto& controller = view.get<CharacterController>(entity);

        // Check for state transitions based on multiple component states
        CheckStateTransitions(entity);

        // Update state machine
        stateMachine.fsm.Update();

        // Sync animation with new state if it changed
        if (g_registry.all_of<Animator>(entity)) {
            UpdateAnimationForState(entity, stateMachine.GetCurrentStateId());
        }
    }
}

void StateMachineSystem::CheckStateTransitions(Entity entity) {
    auto& transform = g_registry.get<Transform>(entity);
    auto& controller = g_registry.get<CharacterController>(entity);
    auto& stateMachine = g_registry.get<StateMachine>(entity);

    // Get input if available
    Input* input = nullptr;
    if (g_registry.all_of<Input>(entity)) {
        input = &g_registry.get<Input>(entity);
    }

    int currentState = stateMachine.GetCurrentStateId();
    int newState = currentState;

    // State transition logic based on multiple components
    if (controller.isGrounded) {
        if (input && (input->currentState.moveLeft || input->currentState.moveRight)) {
            newState = static_cast<int>(CharacterStateType::MOVING);
        } else {
            newState = static_cast<int>(CharacterStateType::IDLE);
        }
    } else {
        if (transform.velocity.y > 0) {
            newState = static_cast<int>(CharacterStateType::JUMPING);
        } else {
            newState = static_cast<int>(CharacterStateType::FALLING);
        }
    }

    if (newState != currentState) {
        stateMachine.ChangeState(newState);
    }
}
```

#### 4. Conditional Component Dependencies

```cpp
// Example: Render system that adapts based on available components
void RenderSystem::Render(RenderQueue& queue) {
    // Basic rendering for entities with Transform and Animator
    auto basicView = g_registry.view<Transform, Animator>();

    for (auto entity : basicView) {
        auto& transform = basicView.get<Transform>(entity);
        auto& animator = basicView.get<Animator>(entity);

        auto* anim = animator.GetCurrentAnimation();
        if (anim == nullptr) continue;

        RenderCommand command;
        command.Sprite = anim->GetCurrentSprite();

        float w = command.Sprite->SourceRect.w * transform.scale.x;
        float h = command.Sprite->SourceRect.h * transform.scale.y;
        float x = transform.position.x - 0.5f * w; // Centered origin
        float y = 720.0f - transform.position.y - h;

        command.DstRect = {x, y, w, h};
        command.Angle = transform.rotation;
        command.Center = {x + w * 0.5f, y + h * 0.5f};
        command.Flip = transform.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

        // Optional: Modify rendering based on Health component
        if (g_registry.all_of<Health>(entity)) {
            auto& health = g_registry.get<Health>(entity);
            if (!health.IsAlive()) {
                // Make dead characters semi-transparent
                command.Alpha = 0.5f;
            } else if (health.invulnerabilityTime > 0.0f) {
                // Flash effect for invulnerable characters
                command.Alpha = (sin(health.invulnerabilityTime * 10.0f) + 1.0f) * 0.5f;
            }
        }

        queue.Push(command);
    }
}
```

#### 5. Component Initialization Dependencies

```cpp
// Example: Factory that sets up component dependencies correctly
Entity CharacterFactory::CreatePlayer() {
    Entity entity = g_registry.create();

    // Add basic components
    auto& transform = g_registry.emplace<Transform>(entity);
    transform.position = {0.0f, 200.0f};
    transform.scale = {2.0f, 2.0f};

    auto& rigidBody = g_registry.emplace<RigidBody>(entity);
    rigidBody.useGravity = true;
    rigidBody.mass = 1.0f;

    auto& controller = g_registry.emplace<CharacterController>(entity);
    controller.moveSpeed = 200.0f;
    controller.jumpForce = 444.0f;

    // Add input handling
    auto& input = g_registry.emplace<Input>(entity);
    input.inputHandler = std::make_unique<PlayerInputHandler>();

    // Add health
    auto& health = g_registry.emplace<Health>(entity);
    health.maxHealth = 100.0f;
    health.currentHealth = 100.0f;

    // Set up animator with state-based animations
    auto& animator = g_registry.emplace<Animator>(entity);
    // Load animations for different states
    LoadAnimationForState(animator, "idle", "assets/player/_Idle.png");
    LoadAnimationForState(animator, "moving", "assets/player/_Run.png");
    LoadAnimationForState(animator, "jumping", "assets/player/_Jump.png");
    LoadAnimationForState(animator, "falling", "assets/player/_Fall.png");
    LoadAnimationForState(animator, "attacking", "assets/player/_Attack.png");
    animator.currentAnimation = "idle";

    // Set up state machine (depends on entity having other components)
    auto& stateMachine = g_registry.emplace<StateMachine>(entity);
    stateMachine.AddState(static_cast<int>(CharacterStateType::IDLE), 
                         std::make_unique<IdleState>(entity, &stateMachine.fsm));
    stateMachine.AddState(static_cast<int>(CharacterStateType::MOVING), 
                         std::make_unique<MovingState>(entity, &stateMachine.fsm));
    stateMachine.AddState(static_cast<int>(CharacterStateType::JUMPING), 
                         std::make_unique<JumpingState>(entity, &stateMachine.fsm));
    stateMachine.AddState(static_cast<int>(CharacterStateType::FALLING), 
                         std::make_unique<FallingState>(entity, &stateMachine.fsm));

    // Start in idle state
    stateMachine.ChangeState(static_cast<int>(CharacterStateType::IDLE));

    return entity;
}
```

### Component Creation and Access

```cpp
// Creating an entity with components
Entity player = g_registry.create();
g_registry.emplace<Transform>(player);
g_registry.emplace<RigidBody>(player);

// Accessing components
auto& transform = g_registry.get<Transform>(player);
transform.position.x = 100.0f;

// Checking if entity has component
if (g_registry.all_of<Transform>(player)) {
    // Entity has Transform component
}
```

### System Implementation with Views

```cpp
void PhysicsSystem::Update() {
    // Process all entities with Transform and RigidBody
    auto view = g_registry.view<Transform, RigidBody>();

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
    }
}
```

### Event System Usage

```cpp
// Define collision event
struct CollisionEvent {
    Entity entityA;
    Entity entityB;
    SDL_FPoint contactPoint;
};

// In collision system
g_registry.ctx().emplace<entt::dispatcher>();
auto& dispatcher = g_registry.ctx().get<entt::dispatcher>();

// Emit collision event
dispatcher.trigger<CollisionEvent>(entityA, entityB, contactPoint);

// Listen for collision events (in another system)
dispatcher.sink<CollisionEvent>().connect<&SomeSystem::OnCollision>();
```

## Migration Strategy

1. Keep existing BaseChar class functional during migration
2. Implement new system alongside old system
3. Gradually migrate features from old to new system
4. Test each phase thoroughly before proceeding
5. Remove old system once new system is fully functional

## Files to Create/Modify

### New Files:

- `ECSWorld.h` (EnTT registry setup)
- `Components.h` (All component definitions)
- `Systems.h/.cpp` (All system implementations)
- `IInputHandler.h`
- `PlayerInputHandler.h/.cpp`
- `AIInputHandler.h/.cpp`
- `CharacterStates.h/.cpp`
- `CharacterFactory.h/.cpp`
- `CharacterManager.h/.cpp`

### Files to Modify:

- `BaseChar.h/.cpp` (gradually phase out)
- `Application.h/.cpp` (integrate new ECS system)
- `FSM.h/.cpp` (minor updates for EnTT integration)
- `CMakeLists.txt` (add EnTT dependency and new source files)

### EnTT Integration Steps:

1. Add EnTT to project via vcpkg: `vcpkg install entt`
2. Update CMakeLists.txt to find and link EnTT
3. Include `<entt/entt.hpp>` in pch.h for performance

This refactoring with EnTT will create a much more flexible, maintainable, and high-performance character system that can easily support different character types, behaviors, and future collision detection.

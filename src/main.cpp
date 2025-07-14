#include <SDL2/SDL.h>
#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 40;
const int BIRD_HEIGHT = 30;
const float GRAVITY = 0.5f;
const float JUMP_STRENGTH = -10.0f;

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    float birdY;
    float birdVelocityY;
    bool isRunning;
    bool spacePressed;
    bool spaceWasPressed;
};

Game game;

void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game.isRunning = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                game.spacePressed = true;
            }
        }
        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                game.spacePressed = false;
            }
        }
    }
}

void update() {
    // Handle jump input (only on space press, not hold)
    if (game.spacePressed && !game.spaceWasPressed) {
        game.birdVelocityY = JUMP_STRENGTH;
    }
    game.spaceWasPressed = game.spacePressed;
    
    // Apply gravity
    game.birdVelocityY += GRAVITY;
    game.birdY += game.birdVelocityY;
    
    // Keep bird within screen bounds
    if (game.birdY < 0) {
        game.birdY = 0;
        game.birdVelocityY = 0;
    }
    if (game.birdY > SCREEN_HEIGHT - BIRD_HEIGHT) {
        game.birdY = SCREEN_HEIGHT - BIRD_HEIGHT;
        game.birdVelocityY = 0;
    }
}

void render() {
    // Clear screen with sky blue background
    SDL_SetRenderDrawColor(game.renderer, 135, 206, 235, 255);
    SDL_RenderClear(game.renderer);
    
    // Draw ground (green rectangle at bottom)
    SDL_SetRenderDrawColor(game.renderer, 34, 139, 34, 255);
    SDL_Rect ground = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};
    SDL_RenderFillRect(game.renderer, &ground);
    
    // Draw bird (yellow rectangle)
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 0, 255);
    SDL_Rect bird = {100, (int)game.birdY, BIRD_WIDTH, BIRD_HEIGHT};
    SDL_RenderFillRect(game.renderer, &bird);
    
    // Draw bird's eye (small black circle approximated with rectangle)
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_Rect eye = {115, (int)game.birdY + 8, 8, 8};
    SDL_RenderFillRect(game.renderer, &eye);
    
    // Draw bird's beak (orange triangle approximated with rectangle)
    SDL_SetRenderDrawColor(game.renderer, 255, 165, 0, 255);
    SDL_Rect beak = {140, (int)game.birdY + 12, 10, 6};
    SDL_RenderFillRect(game.renderer, &beak);
    
    SDL_RenderPresent(game.renderer);
}

void gameLoop() {
    if (!game.isRunning) {
        emscripten_cancel_main_loop();
        return;
    }
    
    handleInput();
    update();
    render();
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // Create window
    game.window = SDL_CreateWindow("Flappy Bird MVP", 
                                   SDL_WINDOWPOS_UNDEFINED, 
                                   SDL_WINDOWPOS_UNDEFINED, 
                                   SCREEN_WIDTH, 
                                   SCREEN_HEIGHT, 
                                   SDL_WINDOW_SHOWN);
    
    if (game.window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    
    // Create renderer
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    if (game.renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return -1;
    }
    
    // Initialize game state
    game.birdY = SCREEN_HEIGHT / 2;
    game.birdVelocityY = 0;
    game.isRunning = true;
    game.spacePressed = false;
    game.spaceWasPressed = false;
    
    std::cout << "Flappy Bird MVP started! Press SPACE to jump!" << std::endl;
    
    // Start the game loop
	
    emscripten_set_main_loop(gameLoop, 60, 1);	
	emscripten_set_main_loop_timing(EM_TIMING_RAF, 60);
    
    // Cleanup (this won't be reached in Emscripten)
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    
    return 0;
}
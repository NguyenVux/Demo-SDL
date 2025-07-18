#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <span>
#include "Animation.h"

// Forward declarations
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

using ResourceID = int64_t;

// Sprite represents a sub-region of a texture with its position and dimensions
struct Sprite {
    SDL_Texture* TexturePtr;
    SDL_Rect SourceRect;
    std::string Name;
};

// SpriteSheet contains a list of Sprite pointers and a name
struct SpriteSheet {
    std::string Name;
    std::vector<const Sprite*> Sprites;
};

struct Texture {
    std::string filePath;
    std::string Name;
    SDL_Texture* TexturePtr;
};


class AssetsManager {
public:
    explicit AssetsManager(SDL_Renderer* renderer);
    ~AssetsManager();

    // Delete copy constructor and assignment operator to prevent accidental copying
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

    // Texture management
    bool LoadTexture(std::string_view filePath);
    SDL_Texture* GetTexture(const std::string& filePath) const;
    bool HasTexture(const std::string& filePath) const;
    bool IsInitialized() const;
    size_t GetTextureCount() const;

    // Sprite management
    bool CreateSprite(const std::string& name, SDL_Texture* texture, const SDL_Rect& rect);
    bool CreateSprite(const std::string& name, const std::string& texture, const SDL_Rect& rect);
    const Sprite* GetSprite(const std::string& name) const;
    bool HasSprite(const std::string& name) const;

    // SpriteSheet management
    bool CreateSpriteSheet(const std::string& name, const std::vector<std::string>& spriteNames);
    const SpriteSheet* GetSpriteSheet(const std::string& name) const;
    bool HasSpriteSheet(const std::string& name) const;

    // Animation management
    bool CreateAnimation(const std::string& name, const std::string& spriteSheetName, int fps, bool loop);
    const Animation* GetAnimation(const std::string& name) const;
    bool HasAnimation(const std::string& name) const;

private:
    // Helper methods
    bool IsValidRenderer() const;

private:
    SDL_Renderer* m_renderer;
    std::unordered_map<std::string, SDL_Texture*> m_textures;
    std::unordered_map<std::string, Sprite> m_sprites;
    std::unordered_map<std::string, SpriteSheet> m_spriteSheets;
    std::unordered_map<std::string, Animation> m_animations;
};

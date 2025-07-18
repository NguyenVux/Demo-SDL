#include "pch.h"
#include "AssetsManager.h"
#include <SDL_image.h>
#include <format>

AssetsManager::AssetsManager(SDL_Renderer* renderer) : 
    m_renderer(renderer)
{
    ASSERT(m_renderer != nullptr,"AssetsManager initialized with null renderer");
}

AssetsManager::~AssetsManager() {

}

bool AssetsManager::LoadTexture(std::string_view filePath) {
    if (!IsValidRenderer()) {
        return false;
    }

    // Check if texture is already loaded
    if (HasTexture(filePath.data())) {
        return true;
    }
    
    // Load texture using SDL_image
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, filePath.data());
    if (texture == nullptr) {
        ASSERT_M(std::format("Failed to load file: {}", filePath).c_str());
        return false;
    }
    
    // Store texture in hashmap
    m_textures[filePath.data()] = texture;
    return true;
}

SDL_Texture* AssetsManager::GetTexture(const std::string& filePath) const
{
    auto texture = m_textures.find(filePath);
    return texture != m_textures.end()?(texture->second):nullptr;
}

bool AssetsManager::HasTexture(const std::string& filePath) const {
    return m_textures.find(filePath) != m_textures.end();
}

bool AssetsManager::IsInitialized() const {
    return m_renderer != nullptr;
}

size_t AssetsManager::GetTextureCount() const {
    return m_textures.size();
}

bool AssetsManager::CreateSprite(const std::string& name, SDL_Texture* texture, const SDL_Rect& rect) {
    if (HasSprite(name)) {
        return false;
    }
    if (texture == nullptr) {
        return false;
    }
    m_sprites[name].TexturePtr = texture;
    m_sprites[name].SourceRect = rect;
    m_sprites[name].Name = name;
    return true;
}

bool AssetsManager::CreateSprite(const std::string& name, const std::string& texture, const SDL_Rect& rect) {
    if (HasSprite(name)) {
        return false;
    }
    return CreateSprite(name, GetTexture(texture), rect);
}

const Sprite* AssetsManager::GetSprite(const std::string& name) const {
    auto iter = m_sprites.find(name);
    if (iter != m_sprites.end()) {
        return &iter->second;
    }
    return nullptr;
}

bool AssetsManager::HasSprite(const std::string& name) const {
    return m_sprites.find(name) != m_sprites.end();
}

bool AssetsManager::CreateSpriteSheet(const std::string& name, const std::vector<std::string>& spriteNames) {
    if (HasSpriteSheet(name)) {
        return false;
    }
    SpriteSheet sheet;
    sheet.Name = name;
    for (const auto& spriteName : spriteNames) {
        auto it = m_sprites.find(spriteName);
        if (it != m_sprites.end()) {
            sheet.Sprites.emplace_back(&it->second);
        }
        // else: skip missing sprite, or optionally log/handle error
    }
    m_spriteSheets.emplace(name, std::move(sheet));
    return true;
}

const SpriteSheet* AssetsManager::GetSpriteSheet(const std::string& name) const {
    auto iter = m_spriteSheets.find(name);
    if (iter != m_spriteSheets.end()) {
        return &iter->second;
    }
    return nullptr;
}

bool AssetsManager::HasSpriteSheet(const std::string& name) const {
    return m_spriteSheets.find(name) != m_spriteSheets.end();
}

bool AssetsManager::IsValidRenderer() const {
    return m_renderer != nullptr;
}

bool AssetsManager::CreateAnimation(const std::string& name, const std::string& spriteSheetName, int fps, bool loop) {
    if (HasAnimation(name)) {
        return false;
    }
    auto sheetIt = m_spriteSheets.find(spriteSheetName);
    if (sheetIt == m_spriteSheets.end()) {
        return false;
    }
    Animation animation;
    animation.SetName(name);
    animation.SetSheet(&sheetIt->second);
    animation.SetFPS(fps);
    animation.SetLoop(loop);
    m_animations.emplace(name, std::move(animation));
    return true;
}

const Animation* AssetsManager::GetAnimation(const std::string& name) const {
    auto iter = m_animations.find(name);
    if (iter != m_animations.end()) {
        return &iter->second;
    }
    return nullptr;
}

bool AssetsManager::HasAnimation(const std::string& name) const {
    return m_animations.find(name) != m_animations.end();
}
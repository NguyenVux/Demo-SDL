#include "pch.h"
#include "AssetsManager.h"

AssetsManager::AssetsManager(SDL_Renderer* renderer) : 
    m_renderer(renderer)
{
    
}

AssetsManager::~AssetsManager() {
    UnloadAllTextures();
}

bool AssetsManager::LoadTexture(const std::string& path) {
    // Check if texture is already loaded
    if (HasTexture(path)) {
        return true;
    }
    
    // Load texture using SDL_image
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
    if (texture == nullptr) {
        // TODO: Add proper logging
        printf("Failed to load texture: %s. SDL_Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    
    // Store texture in hashmap
    m_textures[path] = texture;
    return true;
}

SDL_Texture* AssetsManager::GetTexture(const std::string& path) {
    auto iter = m_textures.find(path);
    if (iter != m_textures.end()) {
        return iter->second;
    }
    
    // Try to load texture if not found
    if (LoadTexture(path)) {
        return m_textures[path];
    }
    
    return nullptr;
}

bool AssetsManager::HasTexture(const std::string& path) const {
    return m_textures.find(path) != m_textures.end();
}

void AssetsManager::UnloadTexture(const std::string& path) {
    auto iter = m_textures.find(path);
    if (iter != m_textures.end()) {
        SDL_DestroyTexture(iter->second);
        m_textures.erase(iter);
    }
}

void AssetsManager::UnloadAllTextures() {
    for (auto& pair : m_textures) {
        SDL_DestroyTexture(pair.second);
    }
    m_textures.clear();
}

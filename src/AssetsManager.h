#pragma once
#include <unordered_map>
#include <string>

class SDL_Texture;
class SDL_Renderer;

class AssetsManager {
public:
    AssetsManager(SDL_Renderer* renderer);
    ~AssetsManager();
    
    // Load texture from file path and store in hashmap
    bool LoadTexture(const std::string& path);
    
    // Get texture by path, returns nullptr if not found
    SDL_Texture* GetTexture(const std::string& path);
    
    // Check if texture exists in manager
    bool HasTexture(const std::string& path) const;
    
    // Unload specific texture
    void UnloadTexture(const std::string& path);
    
    // Unload all textures
    void UnloadAllTextures();

private:
    SDL_Renderer* m_renderer;
    std::unordered_map<std::string, SDL_Texture*> m_textures;
};

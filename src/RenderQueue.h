#pragma once
#include <deque>

struct Sprite;
struct RenderCommand
{
    const Sprite* Sprite;
    SDL_FRect DstRect{};
    double Angle = 0.0;
    SDL_FPoint Center{0.0f, 0.0f};
    SDL_RendererFlip Flip = SDL_FLIP_NONE;
};

class RenderQueue
{
public:
    RenderQueue();
    void Push(const RenderCommand& command);
    bool Dequeue(RenderCommand& command);
    void Clear() { 
        if(!m_commandQueue.empty())
        {
            m_commandQueue.clear(); 
        }
    }
private:
    std::deque<RenderCommand> m_commandQueue;
};

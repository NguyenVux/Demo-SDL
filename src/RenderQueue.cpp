#include "RenderQueue.h"

RenderQueue::RenderQueue() = default;

void RenderQueue::Push(const RenderCommand& command)
{
    m_commandQueue.emplace_back(command);
}

bool RenderQueue::Dequeue(RenderCommand& command)
{
    if (!m_commandQueue.empty())
    {
		command = m_commandQueue.front();
        m_commandQueue.pop_front();
		return true;
    }
	return false;
}
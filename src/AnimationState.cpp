#include "AnimationState.h"

void AnimationState::Enter()
{
    for (auto &animation : m_animations)
    {
        animation.second->Reset();
    }
}

void AnimationState::Exit()
{
}

void AnimationState::Draw(entt::entity entity) const
{
    auto &transform = this->m_registry->get<Transform>(entity);

    if (this->m_animations.find(this->m_currentDirection) == this->m_animations.end())
        return;

    this->m_animations.at(this->m_currentDirection)->Draw(transform);
}
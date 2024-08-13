#pragma once

#include <entt/entt.hpp>

#include "Direction.h"
#include "Animation.h"
#include "AnimationStateMachine.h"

class AnimationStateMachine;

class AnimationState
{
public:
    virtual ~AnimationState() = default;
    virtual void Enter();
    virtual void Update(float deltaTime, entt::entity) = 0;
    virtual void Exit();
    virtual std::shared_ptr<AnimationState> CheckTransitions() = 0;
    virtual void Draw(entt::entity) const;

    void SetDirection(Direction direction) { this->m_currentDirection = direction; }
    void SetRegistry(entt::registry &reg) { this->m_registry = &reg; }

public:
    AnimationStateMachine *m_stateMachine = nullptr;

protected:
    std::unordered_map<Direction, std::shared_ptr<Animation>> m_animations;
    entt::registry *m_registry = nullptr;
    mutable Direction m_currentDirection = Direction::Down;
    mutable Direction m_previousDirection = Direction::Down;
};
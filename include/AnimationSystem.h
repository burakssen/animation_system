#pragma once

#include <entt/entt.hpp>

#include "AnimationComponent.h"

class AnimationSystem
{
public:
    AnimationSystem(entt::registry &registry);
    void Update(float deltaTime);
    void Draw() const;

private:
    entt::registry &m_registry;
};
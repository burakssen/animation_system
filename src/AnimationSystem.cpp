#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(entt::registry &registry) : m_registry(registry) {}

void AnimationSystem::Update(float deltaTime)
{
    this->m_registry.view<AnimationComponent>().each(
        [deltaTime](auto entity, AnimationComponent &animComp)
        {
            auto newState = animComp.currentState->CheckTransitions();
            if (newState && newState != animComp.currentState)
            {
                animComp.currentState->Exit();
                animComp.currentState = newState;
                animComp.currentState->Enter();
            }
            animComp.currentState->Update(deltaTime, entity);
        });
}

void AnimationSystem::Draw() const
{
    this->m_registry.view<AnimationComponent>().each(
        [](auto entity, const AnimationComponent &animComp)
        { animComp.currentState->Draw(entity); });
}
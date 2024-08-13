
#include "AnimationSystem.h"
#include "AnimationComponent.h"
#include "EntityStateComponent.h"

#include <raylib.h>
#include <entt/entt.hpp>

// Define the animation states for each entity separately
class IdleAnimation : public AnimationState
{
public:
    IdleAnimation(std::shared_ptr<Animation> idleDown, std::shared_ptr<Animation> idleUp, std::shared_ptr<Animation> idleRight, std::shared_ptr<Animation> idleLeft);
    void Update(float deltaTime, entt::entity entity) override;
    std::shared_ptr<AnimationState> CheckTransitions() override;
};

class WalkAnimation : public AnimationState
{
public:
    WalkAnimation(std::shared_ptr<Animation> walkDown, std::shared_ptr<Animation> walkUp, std::shared_ptr<Animation> walkRight, std::shared_ptr<Animation> walkLeft);

    void Update(float deltaTime, entt::entity entity) override;
    std::shared_ptr<AnimationState> CheckTransitions() override;
};

class AttackAnimation : public AnimationState
{
public:
    AttackAnimation(std::shared_ptr<Animation> attackDown, std::shared_ptr<Animation> attackUp, std::shared_ptr<Animation> attackRight, std::shared_ptr<Animation> attackLeft);
    void Update(float deltaTime, entt::entity entity) override;
    std::shared_ptr<AnimationState> CheckTransitions() override;
};

class DeathAnimation : public AnimationState
{
public:
    DeathAnimation(std::shared_ptr<Animation> death);
    void Update(float deltaTime, entt::entity entity) override;
    std::shared_ptr<AnimationState> CheckTransitions() override;
};

int main()
{
    InitWindow(800, 450, "Animation System Example");

    SetTargetFPS(60);

    entt::registry registry;

    AnimationSystem animationSystem(registry);

    auto entity = registry.create();
    // Create state machine for each entity
    AnimationStateMachine stateMachine(registry);
    registry.emplace<Transform>(entity, Vector3{400, 225, 0}, Quaternion{0, 0, 0, 0}, Vector3{1, 1, 1});
    registry.emplace<EntityStateComponent>(entity, "player");

    auto &animationComponent = registry.emplace<AnimationComponent>(entity);

    Texture2D idleDownTexture = LoadTexture("resources/idle_down.png");
    Texture2D idleUpTexture = LoadTexture("resources/idle_up.png");
    Texture2D idleRightTexture = LoadTexture("resources/idle_right.png");

    Texture2D walkDownTexture = LoadTexture("resources/walk_down.png");
    Texture2D walkUpTexture = LoadTexture("resources/walk_up.png");
    Texture2D walkRightTexture = LoadTexture("resources/walk_right.png");

    Texture2D attackDownTexture = LoadTexture("resources/attack_down.png");
    Texture2D attackUpTexture = LoadTexture("resources/attack_up.png");
    Texture2D attackRightTexture = LoadTexture("resources/attack_right.png");

    Texture2D deathTexture = LoadTexture("resources/death.png");

    auto idleDown = std::make_shared<Animation>(&idleDownTexture, 6, 0.1f, true);
    auto idleUp = std::make_shared<Animation>(&idleUpTexture, 6, 0.1f, true);
    auto idleRight = std::make_shared<Animation>(&idleRightTexture, 6, 0.1f, true);
    auto idleLeft = std::make_shared<Animation>(&idleRightTexture, 6, 0.1f, true, true);

    auto walkDown = std::make_shared<Animation>(&walkDownTexture, 6, 0.1f, true);
    auto walkUp = std::make_shared<Animation>(&walkUpTexture, 6, 0.1f, true);
    auto walkRight = std::make_shared<Animation>(&walkRightTexture, 6, 0.1f, true);
    auto walkLeft = std::make_shared<Animation>(&walkRightTexture, 6, 0.1f, true, true);

    auto attackDown = std::make_shared<Animation>(&attackDownTexture, 4, 0.15f, false);
    auto attackUp = std::make_shared<Animation>(&attackUpTexture, 4, 0.15f, false);
    auto attackRight = std::make_shared<Animation>(&attackRightTexture, 4, 0.15f, false);
    auto attackLeft = std::make_shared<Animation>(&attackRightTexture, 4, 0.15f, false, true);

    auto death = std::make_shared<Animation>(&deathTexture, 3, 0.2f, false);

    auto idleState = std::make_shared<IdleAnimation>(idleDown, idleUp, idleRight, idleLeft);
    auto walkState = std::make_shared<WalkAnimation>(walkDown, walkUp, walkRight, walkLeft);
    auto attackState = std::make_shared<AttackAnimation>(attackDown, attackUp, attackRight, attackLeft);
    auto deathState = std::make_shared<DeathAnimation>(death);

    stateMachine.AddState("idle", idleState);
    stateMachine.AddState("walk", walkState);
    stateMachine.AddState("attack", attackState);
    stateMachine.AddState("death", deathState);

    stateMachine.SetInitialState("idle");

    animationComponent.currentState = stateMachine.GetCurrentState();

    while (!WindowShouldClose())
    {
        auto &state = registry.get<EntityStateComponent>(entity);

        if (IsKeyDown(KEY_W))
        {
            state.direction = Direction::Up;
        }
        else if (IsKeyDown(KEY_A))
        {
            state.direction = Direction::Left;
        }
        else if (IsKeyDown(KEY_S))
        {
            state.direction = Direction::Down;
        }
        else if (IsKeyDown(KEY_D))
        {
            state.direction = Direction::Right;
        }

        animationSystem.Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);

        animationSystem.Draw();

        EndDrawing();
    }

    UnloadTexture(idleDownTexture);
    UnloadTexture(idleUpTexture);
    UnloadTexture(idleRightTexture);

    UnloadTexture(walkDownTexture);
    UnloadTexture(walkUpTexture);
    UnloadTexture(walkRightTexture);

    UnloadTexture(attackDownTexture);
    UnloadTexture(attackUpTexture);
    UnloadTexture(attackRightTexture);

    UnloadTexture(deathTexture);

    CloseWindow();

    return EXIT_SUCCESS;
}

AttackAnimation::AttackAnimation(std::shared_ptr<Animation> attackDown, std::shared_ptr<Animation> attackUp, std::shared_ptr<Animation> attackRight, std::shared_ptr<Animation> attackLeft)
{
    this->m_animations[Direction::Down] = attackDown;
    this->m_animations[Direction::Up] = attackUp;
    this->m_animations[Direction::Right] = attackRight;
    this->m_animations[Direction::Left] = attackLeft;
}
void AttackAnimation::Update(float deltaTime, entt::entity entity)
{
    auto &state = this->m_registry->get<EntityStateComponent>(entity);
    this->m_previousDirection = this->m_currentDirection;
    this->m_currentDirection = state.direction;
    this->m_animations[this->m_currentDirection]->Update(deltaTime);
}
std::shared_ptr<AnimationState> AttackAnimation::CheckTransitions()
{
    if (this->m_animations[this->m_currentDirection]->IsFinished())
    {
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
        {
            auto newState = this->m_stateMachine->GetStates().at("walk");
            newState->SetDirection(this->m_previousDirection);
            return newState;
        }
        else
        {
            auto newState = this->m_stateMachine->GetStates().at("idle");
            newState->SetDirection(this->m_previousDirection);
            return newState;
        }
    }

    return nullptr;
}

WalkAnimation::WalkAnimation(std::shared_ptr<Animation> walkDown, std::shared_ptr<Animation> walkUp, std::shared_ptr<Animation> walkRight, std::shared_ptr<Animation> walkLeft)
{
    this->m_animations[Direction::Down] = walkDown;
    this->m_animations[Direction::Up] = walkUp;
    this->m_animations[Direction::Right] = walkRight;
    this->m_animations[Direction::Left] = walkLeft;
}

void WalkAnimation::Update(float deltaTime, entt::entity entity)
{
    auto &state = this->m_registry->get<EntityStateComponent>(entity);
    this->m_previousDirection = this->m_currentDirection;
    this->m_currentDirection = state.direction;
    this->m_animations[this->m_currentDirection]->Update(deltaTime);
}

std::shared_ptr<AnimationState> WalkAnimation::CheckTransitions()
{
    if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D))
    {
        auto newState = this->m_stateMachine->GetStates().at("idle");
        newState->SetDirection(this->m_previousDirection);
        return newState;
    }
    else if (IsKeyDown(KEY_SPACE))
    {
        auto newState = this->m_stateMachine->GetStates().at("attack");
        newState->SetDirection(this->m_previousDirection);
        return newState;
    }

    return nullptr;
}

IdleAnimation::IdleAnimation(std::shared_ptr<Animation> idleDown, std::shared_ptr<Animation> idleUp, std::shared_ptr<Animation> idleRight, std::shared_ptr<Animation> idleLeft)
{
    this->m_animations[Direction::Down] = idleDown;
    this->m_animations[Direction::Up] = idleUp;
    this->m_animations[Direction::Right] = idleRight;
    this->m_animations[Direction::Left] = idleLeft;
}

void IdleAnimation::Update(float deltaTime, entt::entity entity)
{
    auto &state = this->m_registry->get<EntityStateComponent>(entity);
    this->m_previousDirection = this->m_currentDirection;
    this->m_currentDirection = state.direction;
    this->m_animations[this->m_currentDirection]->Update(deltaTime);
}

std::shared_ptr<AnimationState> IdleAnimation::CheckTransitions()
{
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
    {
        auto newState = this->m_stateMachine->GetStates().at("walk");
        newState->SetDirection(this->m_previousDirection);
        return newState;
    }
    else if (IsKeyDown(KEY_SPACE))
    {
        auto newState = this->m_stateMachine->GetStates().at("attack");
        newState->SetDirection(this->m_previousDirection);
        return newState;
    }

    return nullptr;
}

DeathAnimation::DeathAnimation(std::shared_ptr<Animation> death)
{
    this->m_animations[Direction::Down] = death;
    this->m_animations[Direction::Up] = death;
    this->m_animations[Direction::Right] = death;
    this->m_animations[Direction::Left] = death;
}

void DeathAnimation::Update(float deltaTime, entt::entity entity)
{
    this->m_animations[this->m_currentDirection]->Update(deltaTime);
}

std::shared_ptr<AnimationState> DeathAnimation::CheckTransitions()
{
    if (this->m_animations[this->m_currentDirection]->IsFinished())
    {
        this->m_registry->destroy(this->m_registry->view<Transform>().front());
    }

    return nullptr;
}
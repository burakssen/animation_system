#include "AnimationStateMachine.h"

void AnimationStateMachine::AddState(const std::string &name, std::shared_ptr<AnimationState> state)
{
    state->m_stateMachine = this;
    state->SetRegistry(this->m_registry);
    this->m_states[name] = state;
}

void AnimationStateMachine::SetInitialState(const std::string &name)
{
    m_currentState = this->m_states[name];
    this->m_currentState->Enter();
}

std::shared_ptr<AnimationState> AnimationStateMachine::GetCurrentState() const
{
    return this->m_currentState;
}
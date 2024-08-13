#pragma once

#include <memory>

#include "AnimationState.h"

struct AnimationComponent
{
    std::shared_ptr<AnimationState> currentState = nullptr;
};
#pragma once

#include <string>

#include <raylib.h>

#include "Direction.h"

struct EntityStateComponent
{
    std::string name;
    Direction direction = Direction::Down;
};
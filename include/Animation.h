#pragma once

// Standard Headers
#include <math.h>

// External Headers
#include <raylib.h>

class Animation
{
public:
    Animation(Texture2D *texture, int frameCount, float frameDuration, bool looping, bool horizontalFlip = false);
    ~Animation();

    void Update(float deltaTime);
    void Draw(const Transform &transform) const;
    bool IsFinished() const;
    void Reset();

private:
    Texture2D *m_spriteSheet = nullptr;
    int m_frameCount = 0;
    float m_frameDuration = 0.0f;
    bool m_looping = false;
    bool m_horizontalFlip = false;
    float m_currentFrame = 0.0f;
    Rectangle m_frameRect = {0.0f, 0.0f, 0.0f, 0.0f};
};
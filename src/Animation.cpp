#include "Animation.h"

Animation::Animation(Texture2D *texture, int frameCount, float frameDuration, bool looping, bool horizontalFlip)
    : m_spriteSheet(texture), m_frameCount(frameCount), m_frameDuration(frameDuration), m_looping(looping), m_horizontalFlip(horizontalFlip), m_currentFrame(0.0f)
{
    this->m_frameRect = {0.0f, 0.0f, static_cast<float>(this->m_spriteSheet->width / frameCount), static_cast<float>(this->m_spriteSheet->height)};
}

Animation::~Animation()
{
}

void Animation::Update(float deltaTime)
{
    this->m_currentFrame += deltaTime / this->m_frameDuration;
    if (this->m_currentFrame >= this->m_frameCount)
    {
        if (this->m_looping)
        {
            this->m_currentFrame = fmod(this->m_currentFrame, this->m_frameCount);
        }
        else
        {
            this->m_currentFrame = fmin(this->m_frameCount - 1, this->m_currentFrame);
        }
    }
}

void Animation::Draw(const Transform &transform) const
{
    Rectangle sourceRect = this->m_frameRect;
    sourceRect.x = static_cast<int>(this->m_currentFrame) * this->m_frameRect.width;
    if (this->m_horizontalFlip)
    {
        sourceRect.width *= -1;
    }
    Vector2 origin = {this->m_frameRect.width * transform.scale.x / 2, this->m_frameRect.height * transform.scale.y / 2};
    DrawTexturePro(
        *this->m_spriteSheet,
        sourceRect,
        {transform.translation.x,
         transform.translation.y,
         this->m_frameRect.width * transform.scale.x,
         this->m_frameRect.height * transform.scale.y},
        origin,
        transform.rotation.x,
        WHITE);
}

bool Animation::IsFinished() const
{
    return !this->m_looping && this->m_currentFrame >= this->m_frameCount - 1;
}

void Animation::Reset()
{
    this->m_currentFrame = 0.0f;
}

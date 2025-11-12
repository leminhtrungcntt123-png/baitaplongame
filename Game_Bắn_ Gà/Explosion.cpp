#include "Explosion.h"

Explosion::Explosion(sf::Vector2f position)
{
    mMaxRadius = 50.f;  // Bán kính tối đa của vụ nổ
    mCurrentRadius = 0.f;
    mDuration = 0.5f;   // Nổ xong trong 0.5 giây
    mTimer = 0.f;
    mIsFinished = false;

    mShape.setPosition(position);
    mShape.setRadius(mCurrentRadius);
    mShape.setFillColor(sf::Color::Transparent); // Ban đầu trong suốt

    // Đặt màu viền là màu vàng (hoặc cam)
    mShape.setOutlineColor(sf::Color(255, 165, 0)); // Màu cam
    mShape.setOutlineThickness(3); // Độ dày viền

    // Đặt tâm của hình tròn vào giữa
    mShape.setOrigin(0, 0); // Sẽ cập nhật sau
}

void Explosion::update(float deltaTime)
{
    if (mIsFinished) return;

    mTimer += deltaTime;
    if (mTimer >= mDuration)
    {
        mIsFinished = true;
        return;
    }

    // Tính toán % tiến trình
    float progress = mTimer / mDuration; // Từ 0.0 -> 1.0

    // Bán kính sẽ tăng từ 0 -> maxRadius
    mCurrentRadius = mMaxRadius * progress;
    mShape.setRadius(mCurrentRadius);

    // Cập nhật lại tâm (Origin) cho đúng khi bán kính thay đổi
    mShape.setOrigin(mCurrentRadius, mCurrentRadius);

    // Độ mờ (alpha) sẽ giảm từ 255 -> 0
    sf::Color color = mShape.getOutlineColor();
    color.a = static_cast<sf::Uint8>(255.f * (1.f - progress));
    mShape.setOutlineColor(color);
}

void Explosion::draw(sf::RenderWindow& window)
{
    if (!mIsFinished)
    {
        window.draw(mShape);
    }
}

bool Explosion::isFinished() const
{
    return mIsFinished;
}

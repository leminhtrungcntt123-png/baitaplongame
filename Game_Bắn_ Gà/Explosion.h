#pragma once
#include <SFML/Graphics.hpp>

class Explosion
{
public:
    // Hàm dựng: Chỉ cần vị trí
    Explosion(sf::Vector2f position);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    // Hàm để Game.cpp biết khi nào nó đã nổ xong
    bool isFinished() const;

private:
    sf::CircleShape mShape;
    float mMaxRadius;
    float mCurrentRadius;
    float mDuration;
    float mTimer;
    bool mIsFinished;
};
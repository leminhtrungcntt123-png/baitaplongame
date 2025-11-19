#include "../header/Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy()
    : Entity(), isRunning(false), speed(100.f),
      waveAmplitude(50.f), waveFrequency(3.f), time(0.f)
{
    velocity = {0.f, 0.f};
    health = 100;
    alive = true;
}

Enemy::Enemy(const sf::Texture *texture, sf::Vector2f pos, float moveSpeed, Vector2f size)
    : Entity(*texture, {0.f, 0.f}, pos, 100),
      isRunning(false),
      speed(moveSpeed),
      waveAmplitude(50.f),
      waveFrequency(3.f),
      time(0.f)
{
    if (!texture)
    {
        std::cerr << "[Enemy] ERROR: Null texture passed!\n";
        return;
    }

    // --- FIX QUAN TRỌNG ---
    const int FRAME_WIDTH = size.x;
    const int FRAME_HEIGHT = size.y;

    animationRun = Animation(texture, FRAME_WIDTH, FRAME_HEIGHT, 0.12f, 0, 3);

    sprite.setTexture(*texture);
    sprite.setTextureRect(animationRun.getCurrentFrame());
    sprite.setScale(0.5f, 0.5f);
    sprite.setPosition(pos);
}

void Enemy::moveZigzag(float dt)
{
    time += dt;

    velocity.x = -speed; // đi sang trái
    velocity.y = std::sin(time * waveFrequency) * waveAmplitude;
}

void Enemy::update(float dt, const sf::RenderWindow &window)
{
    moveZigzag(dt);

    animationRun.update(dt);

    sprite.setTextureRect(animationRun.getCurrentFrame());
    sprite.move(velocity * dt);

    // reset vị trí khi ra khỏi màn hình
    if (sprite.getPosition().x + sprite.getGlobalBounds().width < 0)
    {
        sprite.setPosition(window.getSize().x + 50, sprite.getPosition().y);
        time = 0.f;
    }
}

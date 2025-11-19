#include "../header/Boss.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

Boss::Boss()
    : Enemy(), chargeTime(0), chargeCooldown(3.f), isCharging(false), moveDirection(1)
{
    std::srand(std::time(0));
}

Boss::Boss(const sf::Texture* texture, sf::Vector2f pos, float moveSpeed, sf::Vector2f frameSize)
    : Enemy(texture, pos, moveSpeed, frameSize),
      chargeTime(0), chargeCooldown(3.f), isCharging(false), moveDirection(1)
{
    if (!texture)
    {
        std::cerr << "[Boss] ERROR: Null texture!\n";
        return;
    }

    int w = frameSize.x;
    int h = frameSize.y;

    animationBoss = Animation(texture, w, h, 0.12f, 0, 3);
    
    sprite.setTexture(*texture);
    sprite.setTextureRect(animationBoss.getCurrentFrame());
    sprite.setScale(1.0f, 1.0f);
    sprite.setPosition(pos); // Đặt vị trí ban đầu
    
    // KHÔNG gọi randomizePosition ở đây nữa, sẽ gọi trong update
    moveDirection = (std::rand() % 2 == 0) ? 1 : -1;
}

void Boss::randomizePosition(const sf::Vector2u& windowSize)
{
    // Ngẫu nhiên bên trái hoặc bên phải màn hình
    bool spawnOnRight = std::rand() % 2 == 0;
    
    float x, y;
    
    if (spawnOnRight) {
        // Xuất hiện từ bên phải (ngoài màn hình)
        x = windowSize.x + 100;
    } else {
        // Xuất hiện từ bên trái (ngoài màn hình)
        x = -100;
    }
    
    // Vị trí Y ngẫu nhiên
    y = 50 + std::rand() % (windowSize.y - 200);
    
    sprite.setPosition(x, y);
    
    // Cập nhật hướng di chuyển dựa trên vị trí xuất hiện
    moveDirection = spawnOnRight ? -1 : 1;
}

void Boss::movePattern(float dt, const sf::Vector2f& playerPos)
{
    chargeTime += dt;

    // 1. Boss chuẩn bị lao (charge)
    if (chargeTime > chargeCooldown && !isCharging)
    {
        isCharging = true;
        chargeTime = 0;
    }

    if (isCharging)
    {
        // Boss lao thẳng vào player
        sf::Vector2f direction = playerPos - sprite.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length != 0)
            direction /= length;

        velocity = direction * (speed * 2.5f);

        // kết thúc lao
        if (length < 50.f || chargeTime > 2.0f) // Thêm timeout
        {
            isCharging = false;
        }
        return;
    }

    // 2. Di chuyển zigzag theo hướng ngẫu nhiên
    time += dt;
    velocity.x = speed * 0.7f * moveDirection; // TRÁI hoặc PHẢI
    velocity.y = std::sin(time * waveFrequency) * (waveAmplitude * 1.5f);
}

void Boss::update(float dt, const sf::RenderWindow& window, const sf::Vector2f& playerPos)
{
    // Nếu là lần đầu update, random vị trí
    static bool firstUpdate = true;
    if (firstUpdate)
    {
        randomizePosition(window.getSize());
        firstUpdate = false;
    }

    movePattern(dt, playerPos);

    animationBoss.update(dt);
    sprite.setTextureRect(animationBoss.getCurrentFrame());
    sprite.move(velocity * dt);

    // Reset khi ra khỏi màn hình (cả trái và phải)
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f pos = sprite.getPosition();
    
    if ((moveDirection == -1 && pos.x + bounds.width < -100) || // Ra khỏi trái
        (moveDirection == 1 && pos.x > window.getSize().x + 100)) // Ra khỏi phải
    {
        randomizePosition(window.getSize());
        time = 0;
        isCharging = false;
        chargeTime = 0;
    }
    
    // Giới hạn trong màn hình theo trục Y
    if (pos.y < 50) 
    {
        sprite.setPosition(pos.x, 50);
        velocity.y = std::abs(velocity.y);
    }
    else if (pos.y + bounds.height > window.getSize().y - 50) 
    {
        sprite.setPosition(pos.x, window.getSize().y - bounds.height - 50);
        velocity.y = -std::abs(velocity.y);
    }
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
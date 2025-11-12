#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
public:
    sf::Sprite sprite;

    // --- HÀM DỰNG MỚI (NÂNG CẤP) ---
    // Giờ đây nó nhận Hướng bay, Tốc độ, và Sát thương
    Bullet(sf::Texture& texture, sf::Vector2f startPosition,
        sf::Vector2f direction, float speed, int damage);

    void update(float deltaTime);

    // Hàm mới để trả về sát thương
    int getDamage() const;
    void draw(sf::RenderWindow& window);

private:
    sf::Vector2f mDirection;
    float mSpeed;
    int mDamage;
};

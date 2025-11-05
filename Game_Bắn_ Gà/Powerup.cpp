#include "Powerup.h"

Powerup::Powerup(sf::Texture& texture, sf::Vector2f position, PowerupType type)
{
    this->type = type;
    this->speed = 100.f; // Rơi chậm với tốc độ 100

    this->sprite.setTexture(texture);
    this->sprite.setScale(0.7f, 0.7f); // Thu nhỏ 70%

    // Đặt vị trí ngay tại nơi kẻ thù chết
    this->sprite.setPosition(position);
}

void Powerup::update(float deltaTime)
{
    // Di chuyển vật phẩm đi xuống
    this->sprite.move(0.f, this->speed * deltaTime);
}

void Powerup::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}
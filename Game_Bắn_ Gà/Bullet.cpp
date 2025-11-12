#include "Bullet.h"

// --- TRIỂN KHAI HÀM DỰNG MỚI ---
Bullet::Bullet(sf::Texture& texture, sf::Vector2f startPosition,
    sf::Vector2f direction, float speed, int damage)
{
    // Gán các thuộc tính
    this->mDirection = direction;
    this->mSpeed = speed;
    this->mDamage = damage;

    // Thiết lập hình ảnh
    this->sprite.setTexture(texture);
    this->sprite.setScale(0.8f, 0.8f); // (Bạn có thể chỉnh lại scale)

    // Căn giữa viên đạn tại vị trí bắn
    sf::FloatRect bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    this->sprite.setPosition(startPosition);
}

// --- HÀM UPDATE MỚI (NÂNG CẤP) ---
// Giờ nó di chuyển theo mDirection
void Bullet::update(float deltaTime)
{
    sf::Vector2f movement = this->mDirection * this->mSpeed * deltaTime;
    this->sprite.move(movement);
}

int Bullet::getDamage() const
{
    return this->mDamage;
}
void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}

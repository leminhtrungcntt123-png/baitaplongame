#include "EnemyLvl2.h"

// Hàm Dựng Lvl2:
// Gọi hàm dựng Cha: 2 HP, Tốc độ 120
EnemyLvl2::EnemyLvl2(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(2, 120.f)
{
    // Đặt tỷ lệ rơi đồ (ví dụ: 25% cơ hội rơi UpgradeGun)
    setLoot(25);

    // Lớp con tự thiết lập sprite
    this->sprite.setTexture(texture);
    this->sprite.setScale(1.0f, 1.0f); // (Bạn có thể chỉnh lại scale nếu ảnh to/nhỏ)
    this->sprite.setPosition(startPosition);
    this->scoreValue = 20; // Lvl2 = 20 điểm
}

// Hàm Update Lvl2:
// (Giống hệt Lvl1 và Lvl3)
void EnemyLvl2::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // Chỉ cần gọi hàm của Cha. Cha sẽ tự di chuyển (vào Waypoint -> lượn ngang)
    // và tự gọi hàm shoot() (rỗng) của Cha.
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);
}
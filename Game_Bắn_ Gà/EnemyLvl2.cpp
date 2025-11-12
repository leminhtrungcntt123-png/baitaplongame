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
void EnemyLvl2::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lấy vị trí tâm của quái
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    sf::Vector2f pos(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);

    // Hướng bay (0, 1) = 0 X, 1 Y (đi xuống)
    sf::Vector2f dir(0.f, 1.f);

    // Sát thương (theo yêu cầu của bạn)
    int damage = 2; // Quái Lvl3 gây 2 sát thương

    float bulletSpeed = 400.f; // Tốc độ đạn quái

    // Tạo viên đạn
    enemyBullets.push_back(Bullet(enemyBulletTexture, pos, dir, bulletSpeed, damage));
}

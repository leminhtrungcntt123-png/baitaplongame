#include "EnemyLvl3.h"

// Gọi hàm dựng của Cha: Quái Lvl3 có 5 HP, tốc độ 100
EnemyLvl3::EnemyLvl3(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(5, 100.f)
{
    this->dropType = LootType::UpgradeGun;
    shootCooldown = 1.5f;
    this->sprite.setTexture(texture);
    this->sprite.setScale(1.0f, 1.0f);
    this->sprite.setPosition(startPosition);
    this->scoreValue = 30; // Lvl3 = 50 điểm
}

// Logic update RIÊNG của Lvl3
void EnemyLvl3::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // Gọi hàm update MỚI của Cha
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);
}

// THÊM HÀM NÀY VÀO
void EnemyLvl3::shoot(std::vector<Bullet>& enemyBullets,
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

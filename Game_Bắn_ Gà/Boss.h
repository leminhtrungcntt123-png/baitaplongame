#pragma once
#include "EnemyBase.h"

// Lớp Boss "khổng lồ"
class Boss : public EnemyBase
{
private: // <-- THÊM KHỐI NÀY VÀO
    sf::Texture& mBossBulletTextureRef; // "Túi" riêng để chứa đạn Boss
public:
    // Hàm Dựng
    Boss(sf::Texture& texture, sf::Vector2f startPosition, sf::Texture& bulletTexture);

    // Hàm Update (dùng "lời hứa" 4 tham số)
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth) override;

    // Hàm Shoot (Quan trọng: Boss sẽ "viết đè" (override)
    // logic bắn của Cha để bắn 3 tia)
    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture) override;
};
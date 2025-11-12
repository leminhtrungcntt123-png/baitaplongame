#pragma once
#include "EnemyBase.h" // Kế thừa từ Lớp Cha

// Lớp Quái Cấp 2
class EnemyLvl2 : public EnemyBase
{
public:
    // Hàm Dựng
    EnemyLvl2(sf::Texture& texture, sf::Vector2f startPosition);

    // Hàm Update (dùng "lời hứa" 4 tham số mới)
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth) override;
    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture) override;
};

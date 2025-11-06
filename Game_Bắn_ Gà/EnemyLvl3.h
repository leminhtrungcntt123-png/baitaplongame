#pragma once
#include "EnemyBase.h"

class EnemyLvl3 : public EnemyBase
{
public:
    EnemyLvl3(sf::Texture& texture, sf::Vector2f startPosition);
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth) override;

    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture) override;
};
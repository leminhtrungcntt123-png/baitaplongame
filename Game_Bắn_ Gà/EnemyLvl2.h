#pragma once
#include "EnemyBase.h" 
#include "Animation.h" 

class EnemyLvl2 : public EnemyBase
{
protected:
    // <--- THÊM: Khai báo biến Animation
    Animation animationRun;

public:
    EnemyLvl2(sf::Texture& texture, sf::Vector2f startPosition);

    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth) override;
        
    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture) override;
};


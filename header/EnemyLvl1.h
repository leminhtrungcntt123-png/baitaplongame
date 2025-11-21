#pragma once
#include "EnemyBase.h" // Kế thừa từ Lớp Cha
#include "Animation.h"
// Cú pháp ": public EnemyBase" nghĩa là "Kế thừa"
class EnemyLvl1 : public EnemyBase
{
protected:
    Animation aniamtionRun;

public:
    // Hàm Dựng cho Lvl1
    EnemyLvl1(sf::Texture &texture, sf::Vector2f startPosition);
    // THAY THẾ DÒNG update CŨ BẰNG DÒNG NÀY
    virtual void update(float deltaTime, std::vector<Bullet> &enemyBullets,
                        sf::Texture &enemyBulletTexture, float windowWidth) override;
};

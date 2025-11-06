#include "EnemyLvl1.h"

// Đây là phần "hóc búa": Constructor Của Con Gọi Constructor Của Cha
// Cú pháp ": EnemyBase(1, 150.f)" có nghĩa là:
// "Hãy gọi hàm dựng của Lớp Cha (EnemyBase) và truyền vào
//  HP = 1, Speed = 150.f"
EnemyLvl1::EnemyLvl1(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(1, 150.f) // Quái Lvl1 có 1 HP, tốc độ 150
{
    this->dropType = None;
    // Lớp con tự thiết lập sprite
    this->sprite.setTexture(texture);
    this->sprite.setScale(1.0f, 1.0f);
    this->sprite.setPosition(startPosition);
    this->scoreValue = 10; // Lvl1 = 10 điểm
}
void EnemyLvl1::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // Gọi hàm update MỚI của Cha
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);
}
// KHÔNG CẦN viết hàm update() hay draw()
// vì nó tự động dùng hàm của Lớp Cha (EnemyBase)
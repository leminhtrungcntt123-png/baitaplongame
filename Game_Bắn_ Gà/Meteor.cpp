#include "Meteor.h"
#include <vector> 
#include "Bullet.h" 

// --- HÀM DỰNG (CONSTRUCTOR) CỦA THIÊN THẠCH ---
Meteor::Meteor(sf::Texture& texture, sf::Vector2f position) :
    EnemyBase(5, 150.f)
{
    this->sprite.setTexture(texture);
    this->sprite.setPosition(position);
    this->sprite.setScale(0.8f, 0.8f);

    this->shootCooldown = 99999.f;
    this->shootTimer = 0.f;
    this->dropType = LootType::None;
    this->scoreValue = 5; 
}

// --- "HÀM" (FUNCTION) "UPDATE" (UPDATE) "ĐÃ" (HAS BEEN) "VIẾT ĐÈ" (OVERRIDDEN) ---
void Meteor::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& bulletTexture, float windowWidth)
{
    this->sprite.move(0.f, this->speed * deltaTime);
}

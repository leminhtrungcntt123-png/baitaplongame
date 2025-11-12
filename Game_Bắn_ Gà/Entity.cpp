#include "Entity.h"

// --- HÀM DỰNG "CHA" ---
Entity::Entity(int initialHp, float initialSpeed)
    : Hp(initialHp),
    speed(initialSpeed),
    shootTimer(0.f),
    shootCooldown(1.f) // (Đặt mặc định là 1 giây)
{
    // Hàm dựng Cha không cần làm gì thêm
}

// --- CÁC HÀM "CHUNG" ---

void Entity::takeDamage(int damage)
{
    this->Hp -= damage;
}

bool Entity::isAlive()
{
    return this->Hp > 0;
}

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}

sf::FloatRect Entity::getBounds()
{
    return this->sprite.getGlobalBounds();
}

sf::Vector2f Entity::getPosition()
{
    return this->sprite.getPosition();
}

void Entity::setInitialPosition(float x, float y)
{
    this->sprite.setPosition(x, y);
}

int Entity::getHP()
{
    return this->Hp;
}

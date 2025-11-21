#include "../header/Entity.h"

// --- HÀM DỰNG "CHA" ---
Entity::Entity(int initialHp, float initialSpeed)
    : Hp(initialHp),
    speed(initialSpeed),
    shootTimer(0.f),
    shootCooldown(1.f) ,// (Đặt mặc định là 1 giây)
    mMaxHp(initialHp)
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

int Entity::getMaxHP() const
{
    return mMaxHp;
}

sf::Vector2f Entity::getCenterPosition() const
{
    // (Lấy "ranh giới" (bounds) "toàn cục" (global) "của" (of) "sprite" (sprite))
    sf::FloatRect bounds = this->sprite.getGlobalBounds();

    // (Tính toán "và" (and) "trả về" (return) "tọa độ" (coordinates) "trung tâm" (center))
    return sf::Vector2f(bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);
}

#include "../header/Entity.h"
#include <iostream>

using namespace std;
using namespace sf;

Entity::Entity()
{
    this->velocity = {0.f, 0.f};
    this->health = 100;
    this->alive = true;
}

Entity::Entity(const Texture &texture, Vector2f velocity, Vector2f pos, int health)
{
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    this->velocity = velocity;
    this->health = health;
    this->alive = true;
}

void Entity::setTexture(Texture &texture)
{
    sprite.setTexture(texture);
}
void Entity::setHealth(int HP)
{
    this->health = HP;
}
void Entity::setAlive(bool value)
{
    this->alive = value;
}
void Entity::setPosition(Vector2f pos)
{
    sprite.setPosition(pos);
}
void Entity::setVelocity(Vector2f vel)
{
    this->velocity = vel;
}
Vector2f Entity::getPosition()
    const { return sprite.getPosition(); }
bool Entity::isAlive()
    const { return alive; }

void Entity::update(float dt, const sf::RenderWindow &window)
{
    // 1. Chỉ di chuyển, không giới hạn.
    sprite.move(velocity.x * dt, velocity.y * dt);

    // --- Đã XÓA LOGIC GIỚI HẠN VỊ TRÍ CŨ ---
    // (Logic giới hạn mới, chính xác sẽ được thực hiện trong Player::update)
}

void Entity::draw(RenderWindow &window)
{
    if (alive)
        window.draw(sprite);
}
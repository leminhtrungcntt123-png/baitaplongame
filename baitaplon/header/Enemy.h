#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Animation.h"

class Enemy : public Entity
{
protected:
    Animation animationIdle;
    Animation animationRun;
    bool isRunning;
    float speed;
    float waveAmplitude; // biên độ dao động
    float waveFrequency; // tần số dao động
    float time;          // đếm thời gian để tính sin

public:
    Enemy();
    Enemy(const sf::Texture *texture, sf::Vector2f pos, float moveSpeed, Vector2f size);

    void update(float dt, const sf::RenderWindow &window) override;
    void moveZigzag(float dt);
};

#endif

#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"
#include <SFML/Graphics.hpp>

class Boss : public Enemy
{
private:
    Animation animationBoss;
    float chargeTime;
    float chargeCooldown;
    bool isCharging;
    int moveDirection; // 1 = phải, -1 = trái
    
    void randomizePosition(const sf::Vector2u& windowSize);
    
public:
    Boss();
    Boss(const sf::Texture* texture, sf::Vector2f pos, float moveSpeed, sf::Vector2f frameSize);
    
    void movePattern(float dt, const sf::Vector2f& playerPos);
    void update(float dt, const sf::RenderWindow& window, const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
};

#endif
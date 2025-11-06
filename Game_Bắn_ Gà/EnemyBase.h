#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

class EnemyBase
{
public:
    // Trạng thái AI (Thêm ở Bước 4A)
    enum class AIState { Entering, Attacking };
    enum LootType { None, UpgradeGun };

    sf::Sprite sprite;
    int hp;
    float speed;
    LootType dropType;
    int scoreValue; // Quái này đáng giá bao nhiêu điểm?

    // Constructor
    EnemyBase(int initialHp, float initialSpeed);
    virtual ~EnemyBase() = default;

    // --- Hàm Chung ---
    void takeDamage(int damage);
    bool isAlive();

    // --- Hàm "Ảo" (Virtual) ---
    // (Bản 4 tham số, Thêm ở Bước 5)
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth);

    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture);

    virtual void draw(sf::RenderWindow& window);

    // Hàm set Waypoint (Thêm ở Bước 4A)
    void setWaypoint(sf::Vector2f waypoint);

protected: // <-- Dùng protected
    float mShootTimer;
    float mShootCooldown;

    // Biến AI (Thêm ở Bước 4A & 5)
    AIState mState;
    sf::Vector2f mWaypoint;     // <-- Sửa lỗi E0135 (image_1b92bf)
    float mHorizontalDirection;

    // Hàm Loot (Thêm ở Bước 7 - Sửa lỗi C3861)
    void setLoot(int chance);

}; 
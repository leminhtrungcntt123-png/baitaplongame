#pragma once
#include <vector>
#include "Bullet.h"
#include "Entity.h"

// --- "NÂNG" 2 ENUM RA NGOÀI (Sửa lỗi C2039) ---
enum class AIState { Entering, Attacking };
enum LootType { None, UpgradeGun };

// "Kế thừa" (inherit) từ "Entity"
class EnemyBase : public Entity
{
public:
    // (Biến "public" "riêng")
    LootType dropType;
    int scoreValue;

    // Hàm Dựng
    EnemyBase(int initialHp, float initialSpeed);
    virtual ~EnemyBase() = default;

    // (Hàm "ảo" "riêng")
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth);
    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture);

    // (Hàm "riêng")
    void setWaypoint(sf::Vector2f waypoint);

protected:
    // (Biến "riêng")
    AIState mState;
    sf::Vector2f mWaypoint;
    float mHorizontalDirection;

    // (Hàm "riêng")
    void setLoot(int chance);
};

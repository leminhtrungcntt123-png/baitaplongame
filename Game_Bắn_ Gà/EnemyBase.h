#pragma once
#include <SFML/Graphics.hpp>
#include <vector> // <-- THÊM DÒNG NÀY
#include "Bullet.h" // <-- THÊM DÒNG NÀY

class EnemyBase
{
public:
    enum class AIState { Entering, Attacking };
    enum LootType { None, UpgradeGun };

    sf::Sprite sprite;
    int hp;
    float speed;
    LootType dropType;
    // Constructor (Hàm Dựng)
    EnemyBase(int initialHp, float initialSpeed);

    // Destructor "ảo" (Virtual Destructor)
    // CỰC KỲ QUAN TRỌNG khi dùng kế thừa
    virtual ~EnemyBase() = default;

    // --- Các hàm Chung ---
    // Mọi kẻ thù đều nhận sát thương như nhau
    void takeDamage(int damage);

    // Mọi kẻ thù đều kiểm tra "còn sống" như nhau
    bool isAlive();

    // --- Các hàm "Ảo" (Virtual Functions) ---
    // Cho phép lớp con "viết đè" (override) logic
    // Giờ nó nhận vector đạn của Kẻ thù
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture, float windowWidth);
    // Hàm "ảo" (virtual) cho phép Lớp Con quyết định bắn
    virtual void shoot(std::vector<Bullet>& enemyBullets,
        sf::Texture& enemyBulletTexture);

    // Logic vẽ
    virtual void draw(sf::RenderWindow& window);

    void setWaypoint(sf::Vector2f waypoint);
protected: // <-- THÊM "protected:"
    // (protected: cho phép Lớp Con truy cập)
    float mShootTimer;
    float mShootCooldown;
    AIState mState;         // Trạng thái hiện tại
    sf::Vector2f mWaypoint; // Điểm đến (mục tiêu)
    float mHorizontalDirection; // 1.f = bay sang phải, -1.f = bay sang trái
};
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Entity.h" 

// 2. "Kế thừa" (inherit) từ "Entity"
class Player : public Entity
{
public:
    // Hàm Dựng (Truyền "chung" và "riêng")
    Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
        std::vector<Bullet>& bullets, float bulletSpeed);
    virtual ~Player() = default; // (Hàm Hủy "Ảo")
    void handleInput(sf::Event& event);
    bool update(float deltaTime, float windowWidth); // (Hàm này "viết đè" (override))
    void upgradeGun();
    void resetPosition();

private:
    // (Hàm "riêng")
    void shoot();
    int mGunLevel;
    float mBulletSpeed; // (Vẫn giữ "riêng" vì EnemyBase không cần)
    sf::Vector2f mInitialPos;

    // (Tham chiếu "riêng")
    sf::Texture& mBulletTextureRef;
    std::vector<Bullet>& mPlayerBulletsRef;
};

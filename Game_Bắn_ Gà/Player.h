#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Entity.h"

#include <iostream>
#include <cmath>
// 2. "Kế thừa" (inherit) từ "Entity"
class Player : public Entity
{
public:
    // Hàm Dựng (Truyền "chung" và "riêng")
    Player(sf::Texture &playerTexture, sf::Texture &bulletTexture,
           std::vector<Bullet> &bullets, float bulletSpeed,
           sf::Texture &playerVIPTexture, sf::Texture &bulletVIPTexture);
    virtual ~Player() = default; // (Hàm Hủy "Ảo")
    void handleInput(sf::Event &event);
    bool update(float deltaTime, float windowWidth); // (Hàm này "viết đè" (override))
    void upgradeGun();
    void megaUpgradeGun();
    void resetPosition();
    void heal(int mount); // hàm Buff hp

private:
    // (Hàm "riêng")
    void shoot();
    // bien nay chi den cap do cua hoa luc player ban ra toi da 7
    int mGunLevel;
    // day la mot dac tinh rieng toc do dan cua player cai nay co the do buff tu vat phan nen dat rieng
    float mBulletSpeed; // (Vẫn giữ "riêng" vì EnemyBase không cần)
    // bien truyen vao 2 gia tri thuc chi vi tri ban dau cua player
    sf::Vector2f mInitialPos;
    // bien nay chi toc do cua player khi di ngang;
    float mHorizontalSpeed; // (Biến này bị thiếu)
    // bien nay chi toc do khi di doc cua player
    float mVerticalSpeed;
    // bien nay la de chi frame dac trung cua player
    sf::Texture &mPlayerVIPTextureRef;
    // bien nay de chi dan rieng cua player
    sf::Texture &mBulletPlayerVIPTextureRef;
    bool mIsUpgraded;
    // (Tham chiếu "riêng")
    // bien nay de load file anh dan cua player
    sf::Texture &mBulletTextureRef;
    // bien nay dung de luu vao mang cac vien dan cua player
    std::vector<Bullet> &mPlayerBulletsRef;
    // bien hien thi trang thai di chuyen hay khong cua player
    bool isMoving = false;
    bool lastMovingState = false;
};

#include "Player.h"

// 1. "Thi công" Hàm Dựng MỚI
Player::Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
    std::vector<Bullet>& bullets, float bulletSpeed)
    // 2. "Kết nối" Móng (Entity): 10 HP, 400 Speed
    : Entity(20, 400.f),
    // 3. Khởi tạo biến "RIÊNG"
    mBulletTextureRef(bulletTexture),
    mPlayerBulletsRef(bullets),
    mGunLevel(1),
    mBulletSpeed(bulletSpeed)
{
    // 4. "Thi công" biến "chung" (của "Cha")
    // (DÙNG TÊN "sprite" CỦA BẠN)
    this->sprite.setTexture(playerTexture);
    this->sprite.setScale(1.0f, 1.0f);

    // 5. "Thi công" biến "chung" (của "Cha")
    // (DÙNG TÊN "shoot..." CỦA BẠN)
    this->shootCooldown = 0.5f;
    this->shootTimer = 0.5f;
}

// 6. XÓA BỎ các hàm "chung" (takeDamage, isAlive, draw, getBounds...)
// (Chúng đã được "dời" lên Entity.cpp)

// 7. "Công thức" các hàm "Riêng" (handleInput, upgradeGun...)
void Player::handleInput(sf::Event& event)
{
    // (Để trống - Tự động bắn)
}

void Player::upgradeGun()
{
    if (mGunLevel < 3)
        mGunLevel++;
}

// 8. "Công thức" update (Đã "vá" 100%)
// 8. "Công thức" update (Đã "vá" 100%)
bool Player::update(float deltaTime, float windowWidth)
{
    bool shotFired = false;

    shootTimer += deltaTime; // (Dùng biến "Cha")

    if (shootTimer >= shootCooldown) // (Dùng biến "Cha")
    {
        shootTimer = 0.f;
        shoot();
        shotFired = true;
    }

    float moveDir = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveDir -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveDir += 1.f;

    // (DÙNG TÊN "sprite" VÀ "speed" CỦA BẠN)
    sprite.move(moveDir * speed * deltaTime, 0.f);

    // --- "HỆ THỐNG RANH GIỚI" (BOUNDARY SYSTEM) (ĐÂY LÀ "BẢN VÁ") ---
    sf::FloatRect bounds = sprite.getGlobalBounds();
    if (bounds.left < 0.f)
        sprite.setPosition(0.f, sprite.getPosition().y);
    else if (bounds.left + bounds.width > windowWidth)
        sprite.setPosition(windowWidth - bounds.width, sprite.getPosition().y);
    // --- KẾT THÚC "HỆ THỐNG RANH GIỚI" ---

    return shotFired;
}

// 9. "Công thức" shoot (Đã "vá" 100%)
void Player::shoot()
{
    // (DÙNG TÊN "sprite" CỦA BẠN)
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f shootPos(bounds.left + bounds.width / 2.f, bounds.top);

    if (mGunLevel == 1)
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, shootPos, { 0.f, -1.f }, mBulletSpeed, 1));
    else if (mGunLevel == 2)
    {
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x - 10.f, shootPos.y }, { 0.f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x + 10.f, shootPos.y }, { 0.f, -1.f }, mBulletSpeed, 1));
    }
    else
    {
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, shootPos, { 0.f, -1.f }, mBulletSpeed, 2));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x - 20.f, shootPos.y }, { -0.1f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x + 20.f, shootPos.y }, { 0.1f, -1.f }, mBulletSpeed, 1));
    }
}
// --- THÊM "CÔNG THỨC" BỊ THIẾU NÀY VÀO CUỐI FILE ---
void Player::resetPosition()
{
    // (Chúng ta CHƯA "thi công" mInitialPos,
    //  nên tạm thời để nó reset về giữa màn hình)

    // (DÙNG TÊN "sprite" CỦA BẠN)
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float posX = (800.f - bounds.width) / 2.f;
    float posY = 600.f - bounds.height - 50.f;
    sprite.setPosition(posX, posY);
}

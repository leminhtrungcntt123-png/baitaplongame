#include "Player.h"

// --- HÀM DỰNG (CONSTRUCTOR) ---
// Dùng danh sách khởi tạo để gán các tham chiếu
Player::Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
    std::vector<Bullet>& bullets, float bulletSpeed)
    : mBulletTextureRef(bulletTexture), // Gán tham chiếu texture
    mPlayerBulletsRef(bullets),        // Gán tham chiếu vector đạn
    mGunLevel(1),                     // Tàu tự quản lý cấp súng
    mMoveSpeed(700.f),               // Tàu tự quản lý tốc độ
    mHp(10),                        // Khởi tạo Hp của tàu  
    mMaxHp(10)
{
    // Tàu tự thiết lập sprite của mình
    mSprite.setTexture(playerTexture);
    mSprite.setScale(1.1f, 1.1f);
    this->mBulletSpeed = bulletSpeed;
    this->mShootCooldown = 0.5f; // Bắn 2 viên/giây (bạn có thể chỉnh 0.2f, 0.3f...)
    this->mShootTimer = 0.5f; // Bắn ngay khi game bắt đầu
}

// Hàm này được gọi 1 lần từ Game.cpp
void Player::setInitialPosition(float x, float y)
{
    mSprite.setPosition(x, y);
}

// --- PHÒNG BAN XỬ LÝ INPUT (SỰ KIỆN) ---
void Player::handleInput(sf::Event& event)
{
    // Để trống. Input bắn đã chuyển sang update()
}

// --- PHÒNG BAN CẬP NHẬT LOGIC (TRẠNG THÁI) ---
bool Player::update(float deltaTime, float windowWidth)
{
    bool shotFired = false;

    // 1. Cập nhật đồng hồ bắn (Luôn luôn đếm)
    mShootTimer += deltaTime;

    // 2. XỬ LÝ LOGIC TỰ ĐỘNG BẮN (KHÔNG CẦN NHẤN PHÍM)
    if (mShootTimer >= mShootCooldown)
    {
        mShootTimer = 0.f; // Reset đồng hồ
        shoot();           // Gọi hàm bắn "nội bộ"
        shotFired = true;  // Bật cờ "báo hiệu"
    }

    // 3. XỬ LÝ INPUT DI CHUYỂN (Code cũ của bạn)
    float moveDir = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveDir -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveDir += 1.f;

    // 4. DI CHUYỂN TÀU (Code cũ)
    mSprite.move(moveDir * mMoveSpeed * deltaTime, 0.f);

    // 5. GIỚI HẠN TÀU TRONG MÀN HÌNH (Code cũ)
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    if (bounds.left < 0.f)
        mSprite.setPosition(0.f, mSprite.getPosition().y);
    else if (bounds.left + bounds.width > windowWidth)
        mSprite.setPosition(windowWidth - bounds.width, mSprite.getPosition().y);

    return shotFired; // Báo cho "Nhạc trưởng" biết
}

// --- PHÒNG BAN VẼ ---
void Player::draw(sf::RenderWindow& window)
{
    window.draw(mSprite);
}

// --- HÀM TIỆN ÍCH ---
void Player::upgradeGun()
{
    if (mGunLevel < 3)
    {
        mGunLevel++;
    }
}

sf::FloatRect Player::getBounds() const
{
    return mSprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return mSprite.getPosition();
}

// --- TRIỂN KHAI CÁC HÀM HP MỚI ---
void Player::takeDamage(int damage)
{
    mHp -= damage;
    if (mHp < 0)
    {
        mHp = 0;
    }
}

bool Player::isAlive() const
{
    return mHp > 0;
}

int Player::getHP() const
{
    return mHp;
}

// --- THÊM HÀM MỚI NÀY VÀO CUỐI FILE ---
void Player::shoot()
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    sf::Vector2f shootPos(bounds.left + bounds.width / 2.f, bounds.top);

    if (mGunLevel == 1)
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, shootPos, { 0.f, -1.f }, mBulletSpeed, 1));
    else if (mGunLevel == 2)
    {
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x - 10.f, shootPos.y }, { 0.f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x + 10.f, shootPos.y }, { 0.f, -1.f }, mBulletSpeed, 1));
    }
    else // (Lvl 3 trở lên)
    {
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, shootPos, { 0.f, -1.f }, mBulletSpeed, 2));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x - 20.f, shootPos.y }, { -0.1f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, { shootPos.x + 20.f, shootPos.y }, { 0.1f, -1.f }, mBulletSpeed, 1));
    }
}
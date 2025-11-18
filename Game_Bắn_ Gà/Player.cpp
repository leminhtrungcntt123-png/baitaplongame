#include "Player.h"

// 1. "Thi công" Hàm Dựng MỚI
// --- "THAY THẾ" (REPLACE) "TOÀN BỘ" (THE ENTIRE) "HÀM DỰNG" (CONSTRUCTOR) "CŨ" (OLD) ---
Player::Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
    std::vector<Bullet>& bullets, float bulletSpeed,
    sf::Texture& playerVIPTexture, sf::Texture& bulletVIPTexture) // <-- (Thêm 2 "tham số" (parameters) "mới" (new))

    // 2. "Kết nối" Móng (Entity)
    : Entity(20, 400.f),

    // 3. Khởi tạo biến "RIÊNG" (Cũ)
    mBulletTextureRef(bulletTexture),
    mPlayerBulletsRef(bullets),
    mGunLevel(1),
    mBulletSpeed(bulletSpeed),
    mHorizontalSpeed(0.f),
    mVerticalSpeed(0.f),

    // 4. "VÁ" (PATCH): "Kết nối" (Connect) "các" (the) "biến" (variables) "VIP" (VIP) "mới" (new)
    mPlayerVIPTextureRef(playerVIPTexture),
    mBulletPlayerVIPTextureRef(bulletVIPTexture),
    mIsUpgraded(false) // (Bắt đầu "ở" (at) "trạng thái" (state) "chưa" (not) "nâng cấp" (upgraded))
{
    // (Phần code "setup" (setup) "bên trong" (inside) "{ }" "giữ nguyên" (remains the same))
    this->sprite.setTexture(playerTexture);
    this->sprite.setScale(1.0f, 1.0f);
    this->shootCooldown = 0.5f;
    this->shootTimer = 0.5f;
}

void Player::handleInput(sf::Event& event)
{
    // (Bây giờ "nó" (it) "sẽ" (will) "sử dụng" (use) "các" (the) "biến" (variables) "mới" (new) "mHorizontalSpeed/mVerticalSpeed")
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Left: // Trái
            mHorizontalSpeed = -1.f;
            break;
        case sf::Keyboard::Right: // Phải
            mHorizontalSpeed = 1.f;
            break;
        case sf::Keyboard::Up: // Lên (MỚI)
            mVerticalSpeed = -1.f;
            break;
        case sf::Keyboard::Down: // Xuống (MỚI)
            mVerticalSpeed = 1.f;
            break;
        default:
            break;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Left: // Trái
            if (mHorizontalSpeed < 0) mHorizontalSpeed = 0.f;
            break;
        case sf::Keyboard::Right: // Phải
            if (mHorizontalSpeed > 0) mHorizontalSpeed = 0.f;
            break;
        case sf::Keyboard::Up: // Lên (MỚI)
            if (mVerticalSpeed < 0) mVerticalSpeed = 0.f;
            break;
        case sf::Keyboard::Down: // Xuống (MỚI)
            if (mVerticalSpeed > 0) mVerticalSpeed = 0.f;
            break;
        default:
            break;
        }
    }
}

void Player::upgradeGun()
{
    if (mGunLevel < 3)
        mGunLevel++;
}

// "THAY THẾ" (REPLACE) "TOÀN BỘ" (THE ENTIRE) "HÀM" (FUNCTION) "update" (update) "CŨ" (OLD)
bool Player::update(float deltaTime, float windowWidth)
{
    // --- 1. "VÁ" (PATCH): SỬA LOGIC DI CHUYỂN ---
    this->sprite.move(mHorizontalSpeed * this->speed * deltaTime,
        mVerticalSpeed * this->speed * deltaTime);

    // --- 2. LOGIC RANH GIỚI (BẢN VÁ CHỐNG LAG GÓC) ---
    // Lấy vị trí hiện tại
    sf::Vector2f pos = this->sprite.getPosition();
    // Lấy kích thước tàu
    sf::FloatRect bounds = this->sprite.getGlobalBounds();

    // Tính toán giới hạn (Giả sử màn hình 800x600)
    // Trục X (Trái - Phải)
    if (pos.x < 0.f)
        pos.x = 0.f;
    else if (pos.x + bounds.width > windowWidth)
        pos.x = windowWidth - bounds.width;

    // Trục Y (Trên - Dưới)
    if (pos.y < 0.f)
        pos.y = 0.f;
    else if (pos.y + bounds.height > 600.f)
        pos.y = 600.f - bounds.height;

    // Cập nhật vị trí mới (chỉ 1 lần duy nhất)
    this->sprite.setPosition(pos);

    // Tự động bắn
    shootTimer += deltaTime;
    bool shotFired = false;
    while (shootTimer >= shootCooldown)
    {
        shootTimer -= shootCooldown; // "Trừ" (Subtract) "thời gian" (time) "thay vì" (instead of) "Reset" (Resetting) "về" (to) "0"
        this->shoot();
        shotFired = true; // (Báo "đã" (has) "bắn" (shot))
    }
    return shotFired; // (Báo "không" (has not) "bắn" (shot))
}

// 9. "Công thức" shoot (Đã "vá" 100%)
void Player::shoot()
{
    // 1. "VÁ" (PATCH): "Chọn" (Choose) "Texture" (Texture) "đạn" (bullet) "phù hợp" (correct)
    sf::Texture& currentBulletTexture = mIsUpgraded ? mBulletPlayerVIPTextureRef : mBulletTextureRef;

    // 2. "Lấy" (Get) "vị trí" (position) "trung tâm" (center) (Tối ưu hóa "code" (code) "cũ" (old))
    sf::Vector2f centerPos = this->getCenterPosition();

    // 3. "Xử lý" (Handle) "các" (the) "cấp độ" (levels) "bắn" (shoot)
    switch (mGunLevel)
    {
    case 1:
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.f, -1.f }, mBulletSpeed, 1));
        break;
    case 2:
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos + sf::Vector2f(-15.f, 0.f), { 0.f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos + sf::Vector2f(15.f, 0.f), { 0.f, -1.f }, mBulletSpeed, 1));
        break;
    case 3:
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos + sf::Vector2f(-15.f, 0.f), { -0.1f, -1.f }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos + sf::Vector2f(15.f, 0.f), { 0.1f, -1.f }, mBulletSpeed, 1));
        break;
    case 7:
    {
        float yDir = -1.f; // (Bắn "lên" (up))
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { -0.3f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { -0.2f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { -0.1f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.0f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.1f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.2f, yDir }, mBulletSpeed, 1));
        mPlayerBulletsRef.push_back(Bullet(currentBulletTexture, centerPos, { 0.3f, yDir }, mBulletSpeed, 1));
        break;
    }
    }
}
// --- "THÊM" (ADD) "TOÀN BỘ" (THIS ENTIRE) "HÀM" (FUNCTION) "MỚI" (NEW) "NÀY" (THIS) "VÀO" (INTO) "Player.cpp" ---
void Player::megaUpgradeGun()
{
    mGunLevel = 7; // (Bắn "7" (seven) "tia" (streams))
    this->sprite.setTexture(mPlayerVIPTextureRef); // <-- BIẾN HÌNH TÀU
    mIsUpgraded = true; // <-- Ghi nhớ "trạng thái" (state) "đã" (has been) "nâng cấp" (upgraded)
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

void Player::heal(int mount)
{
    // Cộng thêm máu
    Hp += mount;

    // Đảm bảo không vượt quá giới hạn máu tối đa
    if (Hp > mMaxHp)
    {
        Hp = mMaxHp;
    }
}

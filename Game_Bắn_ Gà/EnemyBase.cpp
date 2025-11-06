#include "EnemyBase.h"
#include <cstdlib> // <-- THÊM DÒNG NÀY (Để dùng rand())
// Code cho Constructor
EnemyBase::EnemyBase(int initialHp, float initialSpeed)
{
    this->hp = initialHp;
    this->speed = initialSpeed;
    this->mShootTimer = 0.f;
    this->mShootCooldown = 2.0f;
    this->dropType = None; // <-- THÊM DÒNG NÀY VÀO
    this->mState = AIState::Entering; // Ban đầu ở trạng thái "Bay vào"
    // Quyết định ngẫu nhiên xem sẽ bay sang trái hay phải
    if (rand() % 2 == 0)
        mHorizontalDirection = 1.f;
    else
        mHorizontalDirection = -1.f;
}

// Code cho hàm nhận sát thương
void EnemyBase::takeDamage(int damage)
{
    this->hp -= damage;
}

// Code cho hàm kiểm tra "còn sống"
bool EnemyBase::isAlive()
{
    return this->hp > 0;
}

void EnemyBase::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // --- 1. LOGIC DI CHUYỂN (AI MỚI) ---
    if (mState == AIState::Entering)
    {
        // Trạng thái 1: Đang bay vào Waypoint (Giữ nguyên)
        sf::Vector2f direction = mWaypoint - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.f)
        {
            mState = AIState::Attacking; // Chuyển trạng thái
            sprite.setPosition(mWaypoint);
        }
        else
        {
            direction /= distance;
            sprite.move(direction * this->speed * deltaTime);
        }
    }
    else if (mState == AIState::Attacking)
    {
        // --- TRẠNG THÁI 2: DI CHUYỂN NGANG (LOGIC MỚI) ---

        // 1. Di chuyển ngang
        sprite.move(mHorizontalDirection * this->speed * deltaTime, 0.f);

        // 2. Kiểm tra "đập" vào tường
        sf::FloatRect bounds = sprite.getGlobalBounds();
        if (bounds.left <= 0.f) // Đập vào tường trái
        {
            mHorizontalDirection = 1.f; // Đổi hướng sang phải
        }
        else if (bounds.left + bounds.width >= windowWidth) // Đập vào tường phải
        {
            mHorizontalDirection = -1.f; // Đổi hướng sang trái
        }
    }

    // --- 2. LOGIC BẮN (GIỮ NGUYÊN) ---
    // Chỉ bắn khi đã vào vị trí (Attacking)
    if (mState == AIState::Attacking)
    {
        mShootTimer += deltaTime;
        if (mShootTimer >= mShootCooldown)
        {
            mShootTimer = 0.f;
            shoot(enemyBullets, enemyBulletTexture);
        }
    }
}
// Code MẶC ĐỊNH cho hàm draw
void EnemyBase::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}
void EnemyBase::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lớp Cha (Base) không bắn
}

void EnemyBase::setWaypoint(sf::Vector2f waypoint)
{
    this->mWaypoint = waypoint;
}

void EnemyBase::setLoot(int chance)
{
    // Lấy 1 số ngẫu nhiên từ 0 đến 99
    int roll = rand() % 100;

    // Nếu "xúc xắc" nhỏ hơn tỷ lệ
    if (roll < chance)
    {
        this->dropType = UpgradeGun;
    }
    else
    {
        this->dropType = None; // Xịt
    }
}
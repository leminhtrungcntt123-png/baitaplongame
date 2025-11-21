#include "../header/EnemyBase.h"
#include <cmath>

// 1. "Kết nối" Móng (gọi Entity)
EnemyBase::EnemyBase(int initialHp, float initialSpeed)
    : Entity(initialHp, initialSpeed) // <-- "Kết nối" Móng
{
    // Khởi tạo biến "RIÊNG"
    mState = AIState::Entering;
    mHorizontalDirection = 1.f;
    dropType = LootType::None;
    scoreValue = 0;
    // (Không cần khởi tạo sprite, hp, speed... "Cha" (Entity) đã làm)
}

// 2. XÓA BỎ các hàm "chung" (takeDamage, isAlive, draw)

// 3. "Công thức" update (Giờ đã "chuẩn" 100% với tên biến của BẠN)
// Logic update "CHUNG" MỚI (Đã "vá" 100%)
void EnemyBase::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // 1. Cập nhật "Hẹn Giờ Bắn" (Dùng biến "Cha")
    shootTimer += deltaTime;
    if (shootTimer >= shootCooldown)
    {
        shootTimer = 0.f;
        // "Ủy thác" (Delegate) "logic" (logic) "bắn" (shoot) "cho" (to) "Con" (Child)
        this->shoot(enemyBullets, enemyBulletTexture);
    }

    // 2. Logic "Di Chuyển" (Dùng biến "Cha")
    if (mState == AIState::Entering)
    {
        sf::Vector2f dir = mWaypoint - this->sprite.getPosition();
        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (distance < 5.f)
        {
            mState = AIState::Attacking;
        }
        else
        {
            dir /= distance;
            this->sprite.move(dir * this->speed * deltaTime);
        }
    }
    else if (mState == AIState::Attacking)
    {
        this->sprite.move(mHorizontalDirection * this->speed * deltaTime, 0.f);

        // --- "HỆ THỐNG RANH GIỚI" (BOUNDARY SYSTEM) (ĐÂY LÀ "BẢN VÁ") ---
        sf::FloatRect bounds = this->sprite.getGlobalBounds();

        if (bounds.left <= 0.f)
        {
            // "LỆNH 1: GÀI" (Snap)
            this->sprite.setPosition(0.f, this->sprite.getPosition().y);

            // "LỆNH 2: ĐỔI HƯỚNG" (Bounce) (DÒNG BỊ THIẾU CỦA BẠN NẰM Ở ĐÂY!)
            mHorizontalDirection = 1.f; // (Ra lệnh: "Di chuyển sang Phải")
        }
        else if (bounds.left + bounds.width >= windowWidth)
        {
            // "LỆNH 1: GÀI" (Snap)
            this->sprite.setPosition(windowWidth - bounds.width, this->sprite.getPosition().y);

            // "LỆNH 2: ĐỔI HƯỚNG" (Bounce)
            mHorizontalDirection = -1.f; // (Ra lệnh: "Di chuyển sang Trái")
        }
    }
}

// 4. "Công thức" shoot (Rỗng)
void EnemyBase::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lớp "Cha" (EnemyBase) không bắn
}

// 5. "Công thức" các hàm "Riêng" (setWaypoint, setLoot)
void EnemyBase::setWaypoint(sf::Vector2f waypoint)
{
    mWaypoint = waypoint;
}

void EnemyBase::setLoot(int chance)
{
    int roll = rand() % 100;
    if (roll < chance)
        this->dropType = UpgradeGun;
    else
        this->dropType = None;
}

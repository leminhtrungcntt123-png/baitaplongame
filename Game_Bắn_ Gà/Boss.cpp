#include "Boss.h"

// Hàm Dựng Boss:
// Gọi hàm dựng Cha: 50 HP, Tốc độ 50 (rất chậm)
Boss::Boss(sf::Texture& texture, sf::Vector2f startPosition, sf::Texture& bulletTexture)
// 2. Thêm vào danh sách khởi tạo
    : EnemyBase(50, 50.f),
    mBossBulletTextureRef(bulletTexture) // <-- LƯU ĐẠN VÀO "TÚI"
{
    // Boss luôn luôn rơi đồ (100% cơ hội)
    setLoot(100);
    this->scoreValue = 100; // Boss = 1000 điểm!

    // Lớp con tự thiết lập sprite
    this->sprite.setTexture(texture);

    // --- YÊU CẦU "HIỆU CHỈNH KÍCH THƯỚC" CỦA BẠN ---
    // (Làm cho Boss "khổng lồ". Bạn có thể chỉnh 2.0f thành 1.5f, 2.5f...)
    this->sprite.setScale(2.0f, 2.0f);

    // Căn giữa Boss khi "ra đời" (dựa trên kích thước ĐÃ scale)
    sf::FloatRect bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    this->sprite.setPosition(startPosition);

    // Boss bắn chậm hơn (1.5 giây/lần)
    shootCooldown = 1.5f;
}

// Hàm Update Boss:
void Boss::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // Chỉ cần gọi hàm của Cha.
    // Cha (EnemyBase) sẽ tự xử lý:
    // 1. Bay vào Waypoint (Trạng thái Entering)
    // 2. Lượn ngang qua lại (Trạng thái Attacking)
    // 3. Tự gọi hàm shoot() (đã được "viết đè") khi "nguội"
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);
}

// Hàm Shoot Boss (Bắn 3 Tia):
void Boss::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lấy tọa độ (đã được căn giữa ở hàm dựng)
    sf::Vector2f pos = this->sprite.getPosition();

    // (Tính toán điểm bắn ở "gầm" Boss, dựa trên scale 2.0)
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    sf::Vector2f shootPos(pos.x, pos.y + bounds.height / 2.f);

    float bulletSpeed = 300.f; // Đạn Boss bay chậm hơn
    int damage = 2; // Đạn Boss 2 damage

    // Bắn 3 viên (Dùng texture đạn của Kẻ thù)
    // Viên 1: Bắn thẳng xuống
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, { 0.f, 1.f }, bulletSpeed, damage));

    // Viên 2: Bắn chéo trái
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, { -0.2f, 1.f }, bulletSpeed, damage));

    // Viên 3: Bắn chéo phải
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, { 0.2f, 1.f }, bulletSpeed, damage));
}

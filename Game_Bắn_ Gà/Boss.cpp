#include "Boss.h"

// Hàm Dựng Boss:
// Gọi hàm dựng Cha: 50 HP, Tốc độ 50 (rất chậm)
Boss::Boss(sf::Texture& texture, sf::Vector2f startPosition, sf::Texture& bulletTexture)
    : EnemyBase(50, 50.f),
    mBossBulletTextureRef(bulletTexture) 
{
    this->scoreValue = 100; // Boss = 1000 điểm!
    this->dropType = LootType::MegaUpgrade;
    shootCooldown = 1.5f; // tốc độ bắn
    this->sprite.setTexture(texture);
    this->sprite.setScale(2.0f, 2.0f);
    this->sprite.setPosition(startPosition);
}

// Hàm Update Boss:
void Boss::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);
}

// Hàm Shoot Boss (Bắn 3 Tia):
void Boss::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lấy tọa độ (Bây giờ là TOP-LEFT)
    sf::Vector2f pos = this->sprite.getPosition();
    // Lấy kích thước
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    // Vị trí X = TopLeft.X + (Nửa Chiều Rộng)
    float shootX = pos.x + bounds.width / 2.f;

    // Vị trí Y = TopLeft.Y + (Cả Chiều Cao)
    float shootY = pos.y + bounds.height;

    // (Đây là tọa độ "GẦM-GIỮA" (Bottom-Center) chính xác)
    sf::Vector2f shootPos(shootX, shootY);

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

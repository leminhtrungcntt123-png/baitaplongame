#include "../header/EnemyLvl2.h"

// 1. CONSTRUCTOR (ĐÃ SỬA)
EnemyLvl2::EnemyLvl2(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(2, 120.f) // 2 HP, Speed 120
{
    setLoot(15); // 18% rớt đồ

    // Setup Sprite cơ bản
    this->sprite.setTexture(texture);
    this->sprite.setScale(0.35f, 0.35f); 
    this->sprite.setPosition(startPosition);
    this->scoreValue = 20; 

    // KHỞI TẠO ANIMATION 
    
    // BƯỚC 1: Điền thông số file ảnh của Quái Level 2
    // (Ví dụ: Ảnh rộng 300px, có 6 hình -> 300/6 = 50)
    int frameWidth = 290;   // 
    int frameHeight = 300;  // 
    int numFrames = 3;     // <--- Số hình động tác bay của Lv2
    float speed = 0.1f;    // Tốc độ vỗ cánh
    
    // BƯỚC 2: Khởi tạo đối tượng animation
    this->animationRun = Animation(&texture, frameWidth, frameHeight, speed, 0, numFrames);

    // BƯỚC 3: Set khung hình đầu tiên
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// 2. UPDATE 
void EnemyLvl2::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // 1. Gọi logic gốc của Cha (di chuyển + tự bắn đạn)
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);

    // CẬP NHẬT ANIMATION
    
    // 2. Tính toán khung hình tiếp theo
    this->animationRun.update(deltaTime);

    // 3. Gán hình ảnh đã cắt vào Sprite
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// 3. SHOOT 
void EnemyLvl2::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Logic bắn đạn giữ nguyên như cũ
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    sf::Vector2f pos(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);
    sf::Vector2f dir(0.f, 1.f);
    int damage = 0; 
    float bulletSpeed = 400.f; 

    enemyBullets.push_back(Bullet(enemyBulletTexture, pos, dir, bulletSpeed, damage,sf::Vector2f(0.6f,0.6f)));
}


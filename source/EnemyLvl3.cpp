#include "../header/EnemyLvl3.h"

// ---------------------------------------------------------
// 1. CONSTRUCTOR
// ---------------------------------------------------------
EnemyLvl3::EnemyLvl3(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(5, 100.f) // 5 HP, Tốc độ 100
{
 
    this->dropType = LootType::UpgradeGun;
       setLoot(20);
    this->shootCooldown = 1.5f;
    this->scoreValue = 30; 

    // Thiết lập Sprite cơ bản
    this->sprite.setTexture(texture);
    this->sprite.setScale(0.4f, 0.4f);
    this->sprite.setPosition(startPosition);

    // -----------------------------------------------------
    // KHỞI TẠO ANIMATION (PHẦN THÊM MỚI)
    // -----------------------------------------------------
    // BƯỚC 1: Điền thông số file ảnh của Quái Level 3
    // Hãy mở ảnh lên và xem chiều rộng/cao của 1 khung hình
    int frameWidth = 290;   // <--- THAY SỐ NÀY (Ví dụ: ảnh rộng 400px, 8 hình -> 50)
    int frameHeight = 300;  // <--- THAY SỐ NÀY
    int numFrames = 3;     // <--- Số lượng hình trong ảnh
    float speed = 0.1f;    // Tốc độ chuyển động
    int row = 0;           // Hàng 0

    // BƯỚC 2: Khởi tạo đối tượng animation
    this->animationRun = Animation(&texture, frameWidth, frameHeight, speed, row, numFrames);

    // BƯỚC 3: Set khung hình đầu tiên ngay lập tức
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// ---------------------------------------------------------
// 2. UPDATE
// ---------------------------------------------------------
void EnemyLvl3::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // 1. Gọi hàm update của Cha (Di chuyển + Tự động Bắn)
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);

    // -----------------------------------------------------
    // CẬP NHẬT ANIMATION (PHẦN THÊM MỚI)
    // -----------------------------------------------------
    // 2. Tính toán frame tiếp theo
    this->animationRun.update(deltaTime);

    // 3. Cập nhật hình ảnh hiển thị lên Sprite
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// ---------------------------------------------------------
// 3. SHOOT (GIỮ NGUYÊN)
// ---------------------------------------------------------
void EnemyLvl3::shoot(std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture)
{
    // Lấy vị trí tâm của quái
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    sf::Vector2f pos(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);

    // Hướng bay (0, 1) = 0 X, 1 Y (đi xuống)
    sf::Vector2f dir(0.f, 1.f);

    // Sát thương
    int damage = 0; 
    float bulletSpeed = 400.f; 

    // Tạo viên đạn
    enemyBullets.push_back(Bullet(enemyBulletTexture, pos, dir, bulletSpeed, damage,sf::Vector2f(0.6f,0.6f)));
}
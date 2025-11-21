#include "../header/EnemyLvl1.h"

// ---------------------------------------------------------
// 1. CONSTRUCTOR
// ---------------------------------------------------------
EnemyLvl1::EnemyLvl1(sf::Texture& texture, sf::Vector2f startPosition)
    : EnemyBase(1, 150.f) // 1 HP, Speed 150
{
    this->dropType = LootType::None;
    
    // Thiết lập Sprite cơ bản
    this->sprite.setTexture(texture);
    this->sprite.setPosition(startPosition);
    this->sprite.setScale(0.3f, 0.3f); 
    this->scoreValue = 10;

    // -----------------------------------------------------
    // KHỞI TẠO ANIMATION (PHẦN BẠN CẦN THÊM)
    // -----------------------------------------------------
    
    // BƯỚC 1: Bạn cần điền thông số thật của ảnh vào đây:
    int frameWidth = 225;   // <--- Chiều rộng 1 khung hình nhỏ (Ví dụ: ảnh rộng 200px có 4 hình => 200/4 = 50)
    int frameHeight = 300;  // <--- Chiều cao 1 khung hình nhỏ
    float speed = 0.1f;    // <--- Tốc độ chuyển hình (0.1s đổi 1 lần)
    int row = 0;           // <--- Hàng nào trong ảnh (thường là 0)
    int numFrames = 3;     // <--- Có bao nhiêu hình trong hàng đó?

    // BƯỚC 2: Gán giá trị cho biến aniamtionRun đã khai báo trong header
    this->aniamtionRun = Animation(&texture, frameWidth, frameHeight, speed, row, numFrames);

    // BƯỚC 3: Cắt khung hình đầu tiên ngay lập tức để tránh hiện cả tấm ảnh to
    this->sprite.setTextureRect(this->aniamtionRun.getCurrentFrame());
}

// ---------------------------------------------------------
// 2. UPDATE
// ---------------------------------------------------------
void EnemyLvl1::update(float deltaTime, std::vector<Bullet>& enemyBullets,
    sf::Texture& enemyBulletTexture, float windowWidth)
{
    // Gọi logic di chuyển của Cha (để nó tự bay xuống)
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);

    // -----------------------------------------------------
    // CẬP NHẬT ANIMATION (PHẦN BẠN CẦN THÊM)
    // -----------------------------------------------------
    
    // 1. Tính toán chuyển frame
    this->aniamtionRun.update(deltaTime);

    // 2. Áp dụng hình cắt mới vào Sprite (để hình thay đổi trên màn hình)
    this->sprite.setTextureRect(this->aniamtionRun.getCurrentFrame());
}
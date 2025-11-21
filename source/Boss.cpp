#include "../header/Boss.h"

// ---------------------------------------------------------
// 1. CONSTRUCTOR
// ---------------------------------------------------------
Boss::Boss(sf::Texture &texture, sf::Vector2f startPosition, sf::Texture &bulletTexture)
    : EnemyBase(50, 50.f),                 // HP = 50, Speed = 50 (Rất trâu và chậm)
      mBossBulletTextureRef(bulletTexture) // Lưu tham chiếu texture đạn riêng
{
    this->scoreValue = 100; // Boss = 100 điểm
    this->dropType = LootType::MegaUpgrade;
    this->shootCooldown = 1.5f; // Tốc độ bắn

    // Thiết lập Sprite
    this->sprite.setTexture(texture);
    this->sprite.setScale(0.8f, 0.8f); // <--- Boss to gấp đôi bình thường
    this->sprite.setPosition(startPosition);

    // -----------------------------------------------------
    // KHỞI TẠO ANIMATION (PHẦN THÊM MỚI)
    // -----------------------------------------------------

    // BƯỚC 1: Điền thông số file ảnh của Boss
    // Lưu ý: Điền kích thước gốc trong file ảnh (chưa nhân đôi)
    int frameWidth = 285;  // <--- VÍ DỤ: Ảnh Boss to, mỗi khung 100px
    int frameHeight = 266; // <--- VÍ DỤ: Chiều cao 100px
    int numFrames = 4;     // <--- Số lượng hình động tác của Boss
    float speed = 0.15f;   // Boss thường cử động chậm hơn quái nhỏ (0.15f hoặc 0.2f)
    int row = 0;           // Hàng 0

    // BƯỚC 2: Khởi tạo
    this->animationRun = Animation(&texture, frameWidth, frameHeight, speed, row, numFrames);

    // BƯỚC 3: Set khung hình đầu tiên
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// ---------------------------------------------------------
// 2. UPDATE
// ---------------------------------------------------------
void Boss::update(float deltaTime, std::vector<Bullet> &enemyBullets,
                  sf::Texture &enemyBulletTexture, float windowWidth)
{
    // 1. Gọi hàm update của Cha (Di chuyển + Tự động kích hoạt bắn)
    EnemyBase::update(deltaTime, enemyBullets, enemyBulletTexture, windowWidth);

    // -----------------------------------------------------
    // CẬP NHẬT ANIMATION
    // -----------------------------------------------------
    // 2. Tính toán frame tiếp theo
    this->animationRun.update(deltaTime);

    // 3. Cập nhật hình ảnh hiển thị
    this->sprite.setTextureRect(this->animationRun.getCurrentFrame());
}

// ---------------------------------------------------------
// 3. SHOOT (GIỮ NGUYÊN logic 3 tia của bạn)
// ---------------------------------------------------------
void Boss::shoot(std::vector<Bullet> &enemyBullets,
                 sf::Texture &enemyBulletTexture)
{
    // Lấy tọa độ hiện tại của Boss
    sf::Vector2f pos = this->sprite.getPosition();
    sf::FloatRect bounds = this->sprite.getGlobalBounds();

    // Tính vị trí "Gầm giữa" để đạn bay ra
    // Lưu ý: Vì Boss dùng setScale 2.0, bounds.width/height đã tự nhân đôi
    float shootX = pos.x + bounds.width / 2.f;
    float shootY = pos.y + bounds.height;
    sf::Vector2f shootPos(shootX, shootY);

    float bulletSpeed = 300.f; // Đạn Boss bay chậm áp lực
    int damage = 0;            // Sát thương

    // --- Bắn 3 viên (Dùng mBossBulletTextureRef đã lưu) ---

    // Viên 1: Bắn thẳng xuống
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, {0.f, 1.f}, bulletSpeed, damage, sf::Vector2f(0.1f, 0.1f)));

    // Viên 2: Bắn chéo trái (Vector -0.2, 1.0)
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, {-0.2f, 1.f}, bulletSpeed, damage, sf::Vector2f(0.1f, 0.1f)));

    // Viên 3: Bắn chéo phải (Vector 0.2, 1.0)
    enemyBullets.push_back(Bullet(mBossBulletTextureRef, shootPos, {0.2f, 1.f}, bulletSpeed, damage, sf::Vector2f(0.1f, 0.1f)));
}
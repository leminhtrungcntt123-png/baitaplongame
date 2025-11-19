#include "../header/Player.h"
#include <iostream>
#include <cmath> // Cần thiết cho std::abs

using std::cerr;
using namespace sf; 
using std::abs; 

// Tỷ lệ scale X ban đầu
const float INITIAL_SCALE_X = 0.35f; 

// ------------------------------------------
// 1. Constructor Mặc Định
// ------------------------------------------
Player::Player()
    : Entity(), isRunning(false) 
{
    velocity = {0.f, 0.f};
    health = 100;
    alive = true;
}

// ------------------------------------------
// 2. Constructor Chính (Khởi tạo Animations và Origin)
// ------------------------------------------
Player::Player(const Texture *texture, Vector2f pos, Vector2f size)
    : Entity(*texture, {0.f, 0.f}, pos, 100), isRunning(false) 
{
    if (!texture)
    {
        cerr << "[Player] ERROR: Null texture passed to constructor!\n";
        return;
    }

    // Kích thước frame 
    const int FRAME_WIDTH = size.x; 
    const int FRAME_HEIGHT = size.y;
    
    // --- KHỞI TẠO ANIMATION ---
    animationIdle = Animation(texture, FRAME_WIDTH, FRAME_HEIGHT, 0.09f, 6, 3); 
    animationRun = Animation(texture, FRAME_WIDTH, FRAME_HEIGHT, 0.08f, 6, 4); 
    
    // B. Cài đặt Sprite ban đầu
    sprite.setTextureRect(animationIdle.getCurrentFrame());
    sprite.setScale(INITIAL_SCALE_X, INITIAL_SCALE_X); 
    
    // *** FIX 1: ĐẶT ORIGIN POINT VỀ TRUNG TÂM (Khắc phục lỗi dịch chuyển lớn) ***
    sprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f); 
    
    sprite.setPosition(pos);
}

// ------------------------------------------
// 3. Hàm update (Logic Chuyển Trạng Thái và Giới hạn)
// ------------------------------------------
void Player::update(float dt, const RenderWindow &window)
{
    // Cập nhật trạng thái chạy
    bool wasRunning = isRunning;
    isRunning = (velocity.x != 0.f || velocity.y != 0.f);

    // Xử lý logic thay đổi trạng thái
    if (wasRunning != isRunning)
    {
        if (isRunning)
            animationRun.reset();
        else
            animationIdle.reset();
    }

    // 1. Update animation
    if (isRunning)
        animationRun.update(dt);
    else
        animationIdle.update(dt);

    // 2. Gán frame vào sprite
    if (isRunning)
    {
        sprite.setTextureRect(animationRun.getCurrentFrame());
    }
    else
    {
        sprite.setTextureRect(animationIdle.getCurrentFrame());
    }
    
    // 3. Lật Sprite theo hướng di chuyển
    if (velocity.x > 0.f) // Di chuyển sang PHẢI
    {
        sprite.setScale(INITIAL_SCALE_X, sprite.getScale().y);
    }
    else if (velocity.x < 0.f) // Di chuyển sang TRÁI
    {
        sprite.setScale(-INITIAL_SCALE_X, sprite.getScale().y);
    }

    // 4. Di chuyển (update Entity) - KHÔNG CÓ GIỚI HẠN Ở ĐÂY
    Entity::update(dt, window);
    
    // *** FIX 2: GIỚI HẠN VỊ TRÍ ĐỂ PLAYER KHÔNG RA KHỎI MÀN HÌNH ***
    
    // Lấy kích thước khung hình
    const int FRAME_WIDTH = animationIdle.getCurrentFrame().width;
    const int FRAME_HEIGHT = animationIdle.getCurrentFrame().height;

    // Chiều cao và Chiều rộng đã scale (Sử dụng std::abs để đảm bảo giá trị dương)
    float scaledHeight = FRAME_HEIGHT * std::abs(sprite.getScale().y);
    float scaledWidth = FRAME_WIDTH * std::abs(sprite.getScale().x);

    // Tính toán các giới hạn (Limit là vị trí TRUNG TÂM của Sprite)
    // Để cạnh dưới/phải chạm ranh giới, trung tâm phải lùi lại nửa kích thước.
    float bottomLimit = (float)window.getSize().y - (scaledHeight / 2.f);
    float topLimit = scaledHeight / 2.f;
    float rightLimit = (float)window.getSize().x - (scaledWidth / 2.f);
    float leftLimit = scaledWidth / 2.f;

    Vector2f newPos = sprite.getPosition();

    // Giới hạn Y (trên/dưới)
    if (newPos.y > bottomLimit) {
        newPos.y = bottomLimit;
    }
    if (newPos.y < topLimit) {
        newPos.y = topLimit;
    }
    
    // Giới hạn X (trái/phải)
    if (newPos.x > rightLimit) {
        newPos.x = rightLimit;
    }
    if (newPos.x < leftLimit) {
        newPos.x = leftLimit;
    }

    // Áp dụng vị trí đã được giới hạn
    sprite.setPosition(newPos);
}
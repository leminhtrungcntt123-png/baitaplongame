#include "../header/Animation.h"
#include <iostream> // Giữ lại nếu bạn vẫn muốn dùng std::cout để debug
using namespace sf;
using namespace std;
// ------------------------------------------
// 1. Constructor Mặc Định
// ------------------------------------------
Animation::Animation()
    : texture(nullptr), frameTime(0.1f), timer(0.f), currentFrame(0), totalFrames(0), loop(true)
{
    // Đã khởi tạo đầy đủ trong danh sách khởi tạo
}

// ------------------------------------------
// 2. Constructor Chính (Đã Tinh Gọn)
// ------------------------------------------
// Chuyển từ (const sf::Texture& tex) sang (const sf::Texture* tex) để phù hợp với thuộc tính
Animation::Animation(const sf::Texture *tex, int frameWidth, int frameHeight, float frameTime, int row, int numFrames)
    : texture(tex), frameTime(frameTime), timer(0.f), currentFrame(0), loop(true)
{
    row = 0;
    if (!texture)
    {
        totalFrames = 0;
        return;
    }

    // *** FIX: Sử dụng numFrames truyền vào thay vì tính toán từ texture ***
    int columns = numFrames;

    if (columns <= 0)
    {
        cerr << "[Animation] ERROR: Invalid numFrames or frame width too large!\n";
        totalFrames = 0;
        return;
    }

    // Tạo các sf::IntRect cho từng frame
    frames.reserve(columns);
    int startY = row * frameHeight;

    for (int x = 0; x < columns; ++x)
    {
        frames.emplace_back(x * frameWidth, startY, frameWidth, frameHeight);
    }

    totalFrames = static_cast<int>(frames.size());
}

// ------------------------------------------
// 3. Hàm update (Logic Thay Thế Frame - Đã Đúng)
// ------------------------------------------
void Animation::update(float dt)
{
    if (frames.empty() || totalFrames <= 0)
        return;

    timer += dt;
    if (timer >= frameTime)
    {
        // Trừ frameTime để giữ lại phần dư của thời gian (đồng bộ tốt hơn)
        timer -= frameTime;

        // CHUYỂN FRAME (REPLACE FRAME)
        currentFrame++;

        // Nếu hết frame
        if (currentFrame >= totalFrames)
        {
            if (loop)
                currentFrame = 0; // Quay lại frame đầu (LẶP LẠI)
            else
                currentFrame = totalFrames - 1; // Giữ ở frame cuối
        }
    }
}

// ------------------------------------------
// 4. Các Hàm Khác (Giữ Nguyên)
// ------------------------------------------
const sf::IntRect &Animation::getCurrentFrame() const
{
    // Cần đảm bảo rằng Animation.h đã định nghĩa 'empty' hoặc đưa 'empty' ra ngoài
    // Tốt nhất là không dùng static local variable nếu muốn clean code.
    // Tuy nhiên, tôi giữ lại để đảm bảo logic của bạn hoạt động.
    static const sf::IntRect empty(0, 0, 0, 0);

    if (frames.empty())
        return empty;

    return frames[currentFrame];
}

void Animation::reset()
{
    currentFrame = 0;
    timer = 0.f;
}

bool Animation::isLoaded() const
{
    return texture != nullptr && totalFrames > 0;
}
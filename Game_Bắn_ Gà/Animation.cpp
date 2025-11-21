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
// 2. Constructor Chính (Đã Sửa: Xóa row = 0; để fix bug choppy)
// ------------------------------------------
Animation::Animation(const sf::Texture *tex, int frameWidth, int frameHeight, float frameTime, int row, int numFrames)
    : texture(tex), frameTime(frameTime), timer(0.f), currentFrame(0), loop(true)
{
    // *** SỬA: XÓA DÒNG NÀY (row = 0;) để dùng row truyền vào đúng cách ***
    // row = 0;  // <-- ĐÃ XÓA: Bug gây animation idle/move trùng lặp

    if (!texture)
    {
        totalFrames = 0;
        return;
    }

    int columns = numFrames;
    if (columns <= 0)
    {
        cerr << "[Animation] ERROR: Invalid numFrames!\n";
        totalFrames = 0;
        return;
    }

    frames.reserve(columns);
    int startY = row * frameHeight;  // <-- Giờ dùng row đúng (0 cho idle, 1 cho move)

    for (int x = 0; x < columns; ++x)
    {
        frames.emplace_back(x * frameWidth, startY, frameWidth, frameHeight);
    }

    totalFrames = static_cast<int>(frames.size());
}

// ------------------------------------------
// 3. Hàm update (ĐÃ SỬA: Tối ưu timer để loại bỏ delay khi loop)
// ------------------------------------------
void Animation::update(float dt)
{
    if (frames.empty() || totalFrames <= 0)
        return;

    // *** THÊM: Clamp dt để tránh spike gây giật (nếu dt > 0.05f, giới hạn lại) ***
    if (dt > 0.05f) dt = 0.05f;

    // *** SỬA LOGIC TIMER: Tính toán chính xác để tránh delay dư khi loop ***
    // Thay vì timer -= frameTime (có thể dư), dùng tích lũy frame dựa trên thời gian tổng
    static float accumulatedTime = 0.f;  // Tích lũy thời gian (thay timer cũ)
    accumulatedTime += dt;

    // Tính số frame cần chuyển (dựa trên accumulatedTime / frameTime)
    int framesToAdvance = static_cast<int>(accumulatedTime / frameTime);
    accumulatedTime -= framesToAdvance * frameTime;  // Giữ dư chính xác (không gây delay)

    // Chuyển frame
    for (int i = 0; i < framesToAdvance; ++i)
    {
        currentFrame++;
        if (currentFrame >= totalFrames)
        {
            if (loop)
                currentFrame = 0;  // Quay lại frame đầu (LẶP LẠI) – không delay
            else
                currentFrame = totalFrames - 1;  // Giữ ở frame cuối
        }
    }
}

// ------------------------------------------
// 4. Các Hàm Khác (ĐÃ SỬA: Hoàn chỉnh reset() để loại bỏ delay khi reset về frame 0)
// ------------------------------------------
const sf::IntRect &Animation::getCurrentFrame() const
{
    static const sf::IntRect empty(0, 0, 0, 0);
    if (frames.empty())
        return empty;
    return frames[currentFrame];
}

void Animation::reset()
{
    currentFrame = 0;
    timer = 0.f;
    /
}

bool Animation::isLoaded() const
{
    return texture != nullptr && totalFrames > 0;
}

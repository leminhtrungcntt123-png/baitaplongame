#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
private:
    const sf::Texture* texture;     // Texture nguồn (đã là con trỏ)
    std::vector<sf::IntRect> frames; // Danh sách frame
    float frameTime;                // Thời gian mỗi frame
    float timer;                    // Bộ đếm thời gian
    int currentFrame;               // Frame hiện tại
    int totalFrames;                // Tổng số frame
    bool loop;                      // Có lặp lại hay không

public:
    Animation();
    
    // Đã sửa: Thay đổi từ tham chiếu (&) sang con trỏ (*)
    // để phù hợp với thuộc tính private 'const sf::Texture* texture;'
    Animation(const sf::Texture* tex, int frameWidth, int frameHeight, float frameTime, int row ,int numFrame); 
    // Tôi cũng thêm giá trị mặc định 'row = 0' cho tham số cuối cùng để linh hoạt hơn.

    // Cập nhật frame theo thời gian
    void update(float dt);

    // Lấy frame hiện tại
    const sf::IntRect& getCurrentFrame() const;

    // Reset animation về frame đầu
    void reset();

    // Kiểm tra animation có load đúng không
    bool isLoaded() const;

    // Cho phép set loop = true/false
    void setLoop(bool enable) { loop = enable; }
};

#endif
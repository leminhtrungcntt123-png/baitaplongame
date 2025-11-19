#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Animation.h"

class Player : public Entity
{
private:
    Animation animationIdle;
    Animation animationRun;

    // Biến trạng thái để kiểm tra xem nhân vật có đang di chuyển không
    bool isRunning; 

public:
    Player();
    
    // Constructor chính: Nhận con trỏ Texture để đồng bộ với thuộc tính
    // texture trong lớp Animation và cách khởi tạo trong Player.cpp
    Player(const Texture* texture,Vector2f pos,Vector2f size);

    // Cập nhật trạng thái và animation (matches Entity::update signature)
    void update(float dt, const RenderWindow &window) override;
    
    // Nếu bạn cần truy cập Sprite cho mục đích debug (như chúng ta đã thảo luận):
    // sf::Sprite& getSprite() { return sprite; } 
    // (Nhớ rằng 'sprite' phải là protected hoặc public trong Entity để dùng cách này)
};

#endif
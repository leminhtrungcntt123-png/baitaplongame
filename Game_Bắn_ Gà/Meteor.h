#pragma once
#include "EnemyBase.h" // (Kế thừa "từ" (from) "Kẻ thù" (Enemy) "Cơ bản" (Base))

// "Thiên thạch" (Meteor) "là" (is) "một" (a) "loại" (type) "Kẻ thù" (Enemy) "Đặc biệt" (Special)
class Meteor : public EnemyBase
{
public:
    // "Hàm Dựng" (Constructor): "Nhận" (Receive) "Texture" (Texture) "và" (and) "Vị trí" (Position) "khởi tạo" (initial)
    Meteor(sf::Texture& texture, sf::Vector2f position);
    virtual ~Meteor() = default;

    // "VIẾT ĐÈ" (OVERRIDE) "hàm" (function) "Update" (Update) "của" (of) "Cha" (Parent)
    // "Chúng ta" (We) "sẽ" (will) "dạy" (teach) "nó" (it) "cách" (how) "chỉ" (to only) "rơi" (fall) "xuống" (down)
    virtual void update(float deltaTime, std::vector<Bullet>& enemyBullets,
        sf::Texture& bulletTexture, float windowWidth) override;
};

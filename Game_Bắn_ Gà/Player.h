#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h" // Player cần biết Bullet là gì

class Player
{
public:
    // Constructor
    Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
        std::vector<Bullet>& bullets);

    // Các "phòng ban" của Player
    bool handleInput(sf::Event& event, float bulletSpeed); // Xử lý sự kiện (bắn)
    void update(float deltaTime, float windowWidth); // Xử lý logic (di chuyển)
    void draw(sf::RenderWindow& window);             // Vẽ

    // --- CÁC HÀM MỚI ---
    void takeDamage(int damage); // Hàm nhận sát thương
    bool isAlive() const;        // Hàm kiểm tra còn sống
    int getHP() const;           // (Tùy chọn) Hàm lấy HP cho UI sau này

    // Các hàm tiện ích
    void upgradeGun();
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setInitialPosition(float x, float y);

private:
    // Dùng con trỏ tham chiếu (&) đến các tài nguyên
    // mà "Game" sở hữu. Player không "sở hữu" chúng.
    sf::Texture& mBulletTextureRef;
    std::vector<Bullet>& mPlayerBulletsRef;

    // Các biến Player tự "sở hữu"
    sf::Sprite mSprite;
    int mGunLevel;
    float mMoveSpeed;
    // --- BIẾN MỚI CHO HP ---
    int mHp;
    int mMaxHp;
};
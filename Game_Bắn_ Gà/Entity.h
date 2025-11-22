#pragma once
#include <SFML/Graphics.hpp>

// Lớp chung cho Player và Enemy kế thừa
class Entity
{
public:
    // Hàm Dựng: Thiết lập các giá trị chung
    Entity(int initialHp, float initialSpeed);

    // QUAN TRỌNG: Hàm Hủy "Ảo" 
    // Bắt buộc phải có khi dùng kế thừa với unique_ptr
    virtual ~Entity() = default;

    // --- CÁC HÀM CHUNG ---
    // 1. Nhận damage
    void takeDamage(int damage);
    // 2. Trạng thái
    bool isAlive();
    sf::Vector2f getCenterPosition() const;

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
    void setInitialPosition(float x, float y);
    // 7. Lấy hp
    int getHP();
    int getMaxHP() const;

protected: // <-- Dùng "protected" để Lớp Con (Player, EnemyBase) "thấy" được
    // --- CÁC BIẾN "CHUNG" (Lấy từ Player/EnemyBase) ---
    sf::Sprite sprite; // (Player dùng "mSprite", Enemy dùng "sprite")
    int Hp;            // (Player dùng "mHp", Enemy dùng "hp")
    float speed;       // (Player dùng "mMoveSpeed", Enemy dùng "speed")
    int mMaxHp;        //hp boss

    float shootTimer;
    float shootCooldown;
};

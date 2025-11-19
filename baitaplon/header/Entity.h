#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity
{
protected:
    Sprite sprite;     // Hình ảnh đại diện của đối tượng
    Vector2f velocity; // Vận tốc di chuyển
    bool alive;        // Trạng thái sống/chết
    int health;        // Máu hoặc chỉ số sinh lực

public:
    Entity();
    Entity(const Texture &texture, Vector2f velocity, Vector2f pos, int health);

    // ==== SETTERS ====
    void setTexture(Texture &texture);
    void setHealth(int HP);
    void setAlive(bool value);
    void setPosition(Vector2f pos);
    void setVelocity(Vector2f vel);

    // ==== GETTERS ====
    Vector2f getPosition() const;
    bool isAlive() const;

    // ==== LOGIC ====
    // Cập nhật chuyển động và giới hạn biên trong cửa sổ
    virtual void update(float deltaTime, const sf::RenderWindow &window);

    // Vẽ đối tượng ra cửa sổ
    virtual void draw(RenderWindow &window);
};

#endif

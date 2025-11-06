#pragma once
#include <SFML/Graphics.hpp>

class Powerup
{
public:
    // Loại vật phẩm (sau này có thể thêm Heal, Shield...)
    enum PowerupType { UpgradeGun };

    sf::Sprite sprite;
    PowerupType type;
    float speed;

    // Hàm dựng
    Powerup(sf::Texture& texture, sf::Vector2f position, PowerupType type);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};
#pragma once
#include "BaseState.h" 
#include <SFML/Graphics.hpp>
#include "Game.h" // Cần game để lấy font và gọi changeState
class GameOverState : public BaseState
{
public:
    GameOverState(sf::Texture &loseBackgroundTexture,Game *game); // "Hàm Dựng" (Constructor)
    virtual void processInput(sf::Event &event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow &window) override;

private:
    // --- "NHÀ KHO RIÊNG" ---

    sf::Font &mFont;

    sf::Texture mLoseBackgroundTexture;
    sf::Sprite mBackgroundSprite; // (Dữ liệu ảnh - Phải nằm ở đây để sống lâu)  // (Khung tranh để vẽ ảnh lên màn hình)

    sf::RectangleShape mOverlay;
    sf::Text mGameOverText;
    sf::Text mRetryButton;
    sf::Text mMenuButton;
};

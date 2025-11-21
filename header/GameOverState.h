#pragma once
#include "BaseState.h" // (Include "Bản Hợp Đồng" (The Contract))
#include <SFML/Graphics.hpp>
#include "Game.h" // (Cần "Game" (Game) "để" (to) "lấy" (get) "Font" (Font) "và" (and) "gọi" (call) "changeState")

// "Chuyên gia" (Specialist) "này" (this) "ký" (signs) "vào" (the) "Bản Hợp Đồng" (Contract) "BaseState" (BaseState)
class GameOverState : public BaseState
{
public:
    GameOverState(sf::Texture &loseBackgroundTexture,Game *game); // "Hàm Dựng" (Constructor)

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐỒNG" (CONTRACT) ---
    virtual void processInput(sf::Event &event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow &window) override;

private:
    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---

    sf::Font &mFont;

    // --- THÊM MỚI VÀO ĐÂY ---
    sf::Texture mLoseBackgroundTexture;
    sf::Sprite mBackgroundSprite; // (Dữ liệu ảnh - Phải nằm ở đây để sống lâu)  // (Khung tranh để vẽ ảnh lên màn hình)
    // ------------------------

    sf::RectangleShape mOverlay;
    sf::Text mGameOverText;
    sf::Text mRetryButton;
    sf::Text mMenuButton;
};

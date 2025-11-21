#pragma once
#include "BaseState.h" // (Include "Bản Hợp Đồng" (The Contract))
#include <SFML/Graphics.hpp>
#include "Game.h" // (Cần "Game" (Game) "để" (to) "lấy" (get) "Font" (Font) "và" (and) "gọi" (call) "changeState")

// "Chuyên gia" (Specialist) "này" (this) "ký" (signs) "vào" (the) "Bản Hợp Đồng" (Contract) "BaseState" (BaseState)
class WonState : public BaseState
{
public:
    WonState(sf::Texture& mWonBackgroundTexture,Game *game); // "Hàm Dựng" (Constructor)

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐÔNG" (CONTRACT) ---
    virtual void processInput(sf::Event &event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow &window) override;

private:
    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---
    sf::Font &mFont;

    // (Các "Đối tượng" (Objects) "UI" (UI) "cụ thể" (specific))
    sf::Texture mWonBackgroundTexture;
    sf::Sprite mBackgroundSprite;
    sf::RectangleShape mOverlay; // Lớp "phủ mờ" (dim overlay)
    sf::Text mWonText;           // Chữ "YOU WIN"

    // (Các "nút" (buttons) "y hệt" (identical) "như" (as) "GameOver" (GameOver))
    sf::Text mRetryButton; // Chữ "CHOI LAI"
    sf::Text mMenuButton;  // Chữ "MENU"
};

#pragma once
#include "BaseState.h" // (Include "Bản Hợp Đồng" (The Contract))
#include <SFML/Graphics.hpp>
#include "Game.h" // (Cần "Game" (Game) "để" (to) "lấy" (get) "Font" (Font) "và" (and) "gọi" (call) "changeState")

// "Chuyên gia" (Specialist) "này" (this) "ký" (signs) "vào" (the) "Bản Hợp Đồng" (Contract) "BaseState" (BaseState)
class GameOverState : public BaseState
{
public:
    GameOverState(Game* game); // "Hàm Dựng" (Constructor)

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐỒNG" (CONTRACT) ---
    virtual void processInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---

    // (Con trỏ "tới" (to) "Tài Sản" (Assets) "chung" (global))
    sf::Font& mFont;

    // (Các "Đối tượng" (Objects) "UI" (UI) "cụ thể" (specific))
    sf::RectangleShape mOverlay;      // Lớp "phủ mờ" (dim overlay)
    sf::Text mGameOverText; // Chữ "GAME OVER"

    // (Các "nút" (buttons) "mới" (new) "của" (of) "bạn" (your))
    sf::Text mRetryButton;  // Chữ "CHOI LAI"
    sf::Text mMenuButton;   // Chữ "MENU"
};

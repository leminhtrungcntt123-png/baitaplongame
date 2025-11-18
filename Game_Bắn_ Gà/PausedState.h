#pragma once
#include "BaseState.h"
#include <SFML/Graphics.hpp>
#include "Game.h" // (Cần "include" (include) "Game" (Game) "để" (to) "lấy" (get) "Font" (Font) "và" (and) "gọi" (call) "popState")

// "Chuyên gia" (Specialist) "này" (this) "ký" (signs) "vào" (the) "Bản Hợp Đồng" (Contract) "BaseState" (BaseState)
class PausedState : public BaseState
{
public:
    PausedState(Game* game); // "Hàm Dựng" (Constructor)

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐỒNG" (CONTRACT) ---
    virtual void processInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---
    // (Các "biến" (variables) "Pause" (Pause) "đã" (have) "di chuyển" (moved) "đến" (to) "đây" (here) "từ" (from) "Game.h" "cũ" (old))

    // (Con trỏ "tới" (to) "Tài Sản" (Assets) "chung" (global))
    sf::Font& mFont;

    // (Các "Đối tượng" (Objects) "UI" (UI) "cụ thể" (specific))
    sf::RectangleShape mPauseOverlay; // Lớp "phủ mờ" (dim overlay)
    sf::Text mPauseResumeButton;      // Chữ "CHƠI TIẾP"
    sf::Text mPauseExitButton;        // Chữ "THOÁT"
};

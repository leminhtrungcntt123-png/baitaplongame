#pragma once
#include "BaseState.h" 
#include <SFML/Graphics.hpp>
#include "Game.h" 

class MainMenuState : public BaseState
{
public:
    // "Hàm Dựng" (Constructor): "Nhận" (Receive) "Nhạc trưởng" (Game)
    MainMenuState(Game* game);

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐỒNG" (CONTRACT) ---
    virtual void processInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---
    // (Đây "là" (is) "nơi" (where) "các" (the) "biến" (variables) "bị" (were) "mất" (missing) "đã" (have) "di chuyển" (moved) "đến" (to))

    // (Lấy "con trỏ" (pointers) "tới" (to) "các" (the) "Tài Sản" (Assets) "toàn cục" (global))
    sf::Texture& mMenuBackgroundTexture;
    sf::Texture& mTitleTexture;
    sf::Texture& mPlayButtonTexture;

    // (Lưu "các" (the) "Sprite" (Sprites) "cụ thể" (specific) "của" (of) "Menu" (Menu))
    sf::Sprite  mMenuBackgroundSprite;
    sf::Sprite  mTitleSprite;
    sf::Sprite  mPlayButtonSprite;
};

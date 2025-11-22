#pragma once
#include <SFML/Graphics.hpp>
#include <memory> 
class Game;

class BaseState
{
public:
    // Hàm dựng lưu lại con trỏ "Game"
    BaseState(Game* game) : mGame(game) {}

    // "Hàm Hủy" ảo (virtual) bắt buộc phải có
    virtual ~BaseState() {}

    // "Việc 1: Xử lý Input" (Job 1: Process Input)
    virtual void processInput(sf::Event& event) = 0;

    // "Việc 2: Cập nhật Logic" (Job 2: Update Logic)
    virtual void update(float deltaTime) = 0;

    // "Việc 3: Vẽ" (Job 3: Render)
    // (Chúng ta truyền cửa sổ window vào hàm render)
    virtual void render(sf::RenderWindow& window) = 0;

protected:
    // Một con trỏ để giao tiếp với Game
    // (ví dụ: mGame->pushState(...) hoặc mGame->mWindow)
    Game* mGame;
};

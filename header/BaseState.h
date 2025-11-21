#pragma once
#include <SFML/Graphics.hpp>
#include <memory> 

// (Chúng ta "Forward Declare" Game để "tránh" (avoid) "lỗi" (bug) "include" (include) "vòng tròn" (circular))
class Game;

class BaseState
{
public:
    // Hàm dựng lưu lại con trỏ "Game"
    BaseState(Game* game) : mGame(game) {}

    // "Hàm Hủy" ảo (virtual) bắt buộc phải có
    virtual ~BaseState() {}

    // (Hàm "ảo" (virtual) "thuần túy" (pure) (ký hiệu "= 0"))

    // "Việc 1: Xử lý Input" (Job 1: Process Input)
    virtual void processInput(sf::Event& event) = 0;

    // "Việc 2: Cập nhật Logic" (Job 2: Update Logic)
    virtual void update(float deltaTime) = 0;

    // "Việc 3: Vẽ" (Job 3: Render)
    // (Chúng ta "truyền" (pass) "cửa sổ" (window) "vào" (into) "hàm" (function) "vẽ" (render))
    virtual void render(sf::RenderWindow& window) = 0;

protected:
    // "Một" (A) "con trỏ" (pointer) "để" (to) "Chuyên gia" (Specialist) "giao tiếp" (talk) "với" (with) "Nhạc trưởng" (Game)
    // (ví dụ: mGame->pushState(...) "hoặc" (or) mGame->mWindow)
    Game* mGame;
};

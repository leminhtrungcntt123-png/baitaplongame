#include "GameOverState.h"
#include "Game.h"
#include "PlayingState.h"  
#include "MainMenuState.h" 

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA GAME OVER ---
GameOverState::GameOverState(Game* game) :
    BaseState(game),     // 1. "Kết nối" (Connect) "Hợp đồng" (Contract)
    mFont(game->mFont)   // 2. "Kết nối" (Connect) "Tài Sản" (Assets) "Font" (Font)
{
    // 3. "THI CÔNG" (IMPLEMENT) "SETUP" (SETUP) "UI" (UI)
    // 3a. Setup Lớp Phủ Mờ
    mOverlay.setSize(sf::Vector2f(800.f, 600.f));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // 3b. Setup Chữ "GAME OVER"
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(64);
    mGameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect loseBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(loseBounds.left + loseBounds.width / 2.f,
        loseBounds.top + loseBounds.height / 2.f);
    mGameOverText.setPosition(800.f / 2.f, 600.f / 2.f - 100.f); // (Đẩy "lên" (up) "cao" (high) "một chút" (a bit))

    // 3c. Setup Nút "CHƠI LẠI" (Retry) MỚI
    mRetryButton.setFont(mFont);
    mRetryButton.setString("RETRY");
    mRetryButton.setCharacterSize(40);
    mRetryButton.setFillColor(sf::Color::White);
    sf::FloatRect retryBounds = mRetryButton.getLocalBounds();
    mRetryButton.setOrigin(retryBounds.left + retryBounds.width / 2.f,
        retryBounds.top + retryBounds.height / 2.f);
    mRetryButton.setPosition(800.f / 2.f, 600.f / 2.f + 0.f); // (Ở "giữa" (center))

    // 3d. Setup Nút "MENU" MỚI
    mMenuButton.setFont(mFont);
    mMenuButton.setString("MENU");
    mMenuButton.setCharacterSize(40);
    mMenuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuBounds = mMenuButton.getLocalBounds();
    mMenuButton.setOrigin(menuBounds.left + menuBounds.width / 2.f,
        menuBounds.top + menuBounds.height / 2.f);
    mMenuButton.setPosition(800.f / 2.f, 600.f / 2.f + 70.f); // (Ở "dưới" (below) "nút" (button) "Retry" (Retry))
}

// --- "ĐIỀU KHOẢN 1" (CLAUSE 1): PROCESSINPUT ---
void GameOverState::processInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

            // "Click" (Click) "CHƠI LẠI"?
            if (mRetryButton.getGlobalBounds().contains(mousePos))
            {
                // "Ra lệnh" (Command) "cho" (the) "Nhạc trưởng" (Game): "Thay" (Change) "Chuyên gia" (Specialist) "sang" (to) "PlayingState" (PlayingState) "MỚI" (NEW)
                mGame->changeState(std::make_unique<PlayingState>(mGame));
            }
            // "Click" (Click) "MENU"?
            else if (mMenuButton.getGlobalBounds().contains(mousePos))
            {
                // "Ra lệnh" (Command) "cho" (the) "Nhạc trưởng" (Game): "Thay" (Change) "Chuyên gia" (Specialist) "sang" (to) "MainMenuState" (MainMenuState) "MỚI" (NEW)
                mGame->changeState(std::make_unique<MainMenuState>(mGame));
            }
        }
    }
}


// --- "ĐIỀU KHOẢN 2" (CLAUSE 2): UPDATE ---
void GameOverState::update(float deltaTime)
{
    // (Thêm "logic" (logic) "Hover" (Hover) "cho" (for) "các" (the) "nút" (buttons) "mới" (new))
    sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

    // Hover "CHƠI LẠI"
    if (mRetryButton.getGlobalBounds().contains(mousePos))
        mRetryButton.setFillColor(sf::Color::Yellow);
    else
        mRetryButton.setFillColor(sf::Color::White);

    // Hover "MENU"
    if (mMenuButton.getGlobalBounds().contains(mousePos))
        mMenuButton.setFillColor(sf::Color::Yellow);
    else
        mMenuButton.setFillColor(sf::Color::White);
}

// --- "ĐIỀU KHOẢN 3" (CLAUSE 3): RENDER ---
void GameOverState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());

    window.draw(mOverlay);      // 1. Vẽ Lớp Phủ Mờ (để "tạo" (create) "nền" (background) "tối" (dark))
    window.draw(mGameOverText); // 2. Vẽ Chữ "GAME OVER"
    window.draw(mRetryButton);  // 3. Vẽ Nút "Chơi Lại"
    window.draw(mMenuButton);   // 4. Vẽ Nút "Menu"
}

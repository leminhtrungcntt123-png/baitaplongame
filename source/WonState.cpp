#include "../header/WonState.h"
#include "../header/Game.h"
#include "../header/PlayingState.h"  // (Cần "include" (include) "để" (to) "biết" (know) "cách" (how) "CHƠI LẠI" (RETRY))
#include "../header/MainMenuState.h" // (Cần "include" (include) "để" (to) "biết" (know) "cách" (how) "VỀ MENU" (RETURN TO MENU))

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA CHIẾN THẮNG ---
WonState::WonState(sf::Texture &WonBackgroundTexture, Game *game) : BaseState(game),   // 1. "Kết nối" (Connect) "Hợp đồng" (Contract)
                                                                    mFont(game->mFont) // 2. "Kết nối" (Connect) "Tài Sản" (Assets) "Font" (Font)
{
    // 3. "THI CÔNG" (IMPLEMENT) "SETUP" (SETUP) "UI" (UI)
    mWonBackgroundTexture = WonBackgroundTexture;
    mBackgroundSprite.setTexture(mWonBackgroundTexture);
    sf::Vector2u textureSize = mWonBackgroundTexture.getSize();
    sf::Vector2u windowSize = mGame->mWindow.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    mBackgroundSprite.setScale(scaleX, scaleY);
    // 3a. Setup Lớp Phủ Mờ
    mOverlay.setSize(sf::Vector2f(800.f, 600.f));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // 3b. Setup Chữ "YOU WIN"
    mWonText.setFont(mFont);
    mWonText.setString("VICTORY!");
    mWonText.setCharacterSize(64);
    mWonText.setFillColor(sf::Color::Yellow); // (Màu Vàng)
    sf::FloatRect winBounds = mWonText.getLocalBounds();
    mWonText.setOrigin(winBounds.left + winBounds.width / 2.f,
                       winBounds.top + winBounds.height / 2.f);
    mWonText.setPosition(800.f / 2.f, 600.f / 2.f - 100.f); // (Đẩy "lên" (up) "cao" (high))

    // 3c. Setup Nút "CHƠI LẠI" (Retry)
    mRetryButton.setFont(mFont);
    mRetryButton.setString("CHOI LAI");
    mRetryButton.setCharacterSize(40);
    mRetryButton.setFillColor(sf::Color::White);
    sf::FloatRect retryBounds = mRetryButton.getLocalBounds();
    mRetryButton.setOrigin(retryBounds.left + retryBounds.width / 2.f,
                           retryBounds.top + retryBounds.height / 2.f);
    mRetryButton.setPosition(800.f / 2.f, 600.f / 2.f + 0.f); // (Ở "giữa" (center))

    // 3d. Setup Nút "MENU"
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
void WonState::processInput(sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

            // "Click" (Click) "CHƠI LẠI"?
            if (mRetryButton.getGlobalBounds().contains(mousePos))
            {
                mGame->changeState(std::make_unique<PlayingState>(mGame));
            }
            // "Click" (Click) "MENU"?
            else if (mMenuButton.getGlobalBounds().contains(mousePos))
            {
                mGame->changeState(std::make_unique<MainMenuState>(mGame));
            }
        }
    }
}

// --- "ĐIỀU KHOẢN 2" (CLAUSE 2): UPDATE ---
void WonState::update(float deltaTime)
{
    // (Thêm "logic" (logic) "Hover" (Hover) "y hệt" (identical) "như" (as) "GameOver" (GameOver))
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
void WonState::render(sf::RenderWindow &window)
{
    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mOverlay);     // 1. Vẽ Lớp Phủ Mờ
    window.draw(mWonText);     // 2. Vẽ Chữ "YOU WIN"
    window.draw(mRetryButton); // 3. Vẽ Nút "Chơi Lại"
    window.draw(mMenuButton);  // 4. Vẽ Nút "Menu"
}

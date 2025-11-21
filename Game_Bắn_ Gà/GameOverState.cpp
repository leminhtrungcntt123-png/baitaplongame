#include "../header/GameOverState.h"
#include "../header/Game.h"
#include "../header/PlayingState.h"
#include "../header/MainMenuState.h"

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA GAME OVER ---
GameOverState::GameOverState(sf::Texture &loseBackgroundTexture, Game *game)
    : BaseState(game),
      mFont(game->mFont)
{
    // --- 1. SETUP BACKGROUND ---

    // Bước quan trọng: Copy dữ liệu từ tham số truyền vào (loseBackgroundTexture)
    // sang biến "nhà kho" của riêng class này (mLoseBackgroundTexture)
    // để đảm bảo ảnh không bị mất khi ra khỏi hàm.
    mLoseBackgroundTexture = loseBackgroundTexture;

    // Gán Texture của riêng mình vào khung tranh (Sprite)
    mBackgroundSprite.setTexture(mLoseBackgroundTexture);

    // Co giãn ảnh cho vừa màn hình (Scale)
    sf::Vector2u textureSize = mLoseBackgroundTexture.getSize();
    sf::Vector2u windowSize = mGame->mWindow.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    mBackgroundSprite.setScale(scaleX, scaleY);

    // --- 2. SETUP LỚP PHỦ MỜ ---
    mOverlay.setSize(sf::Vector2f(800.f, 600.f));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // --- 3. SETUP UI (CHỮ VÀ NÚT) ---

    // Chữ GAME OVER
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(64);
    mGameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect loseBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(loseBounds.left + loseBounds.width / 2.f,
                            loseBounds.top + loseBounds.height / 2.f);
    mGameOverText.setPosition(800.f / 2.f, 600.f / 2.f - 100.f);

    // Nút RETRY
    mRetryButton.setFont(mFont);
    mRetryButton.setString("RETRY");
    mRetryButton.setCharacterSize(40);
    mRetryButton.setFillColor(sf::Color::White);
    sf::FloatRect retryBounds = mRetryButton.getLocalBounds();
    mRetryButton.setOrigin(retryBounds.left + retryBounds.width / 2.f,
                           retryBounds.top + retryBounds.height / 2.f);
    mRetryButton.setPosition(800.f / 2.f, 600.f / 2.f + 0.f);

    // Nút MENU
    mMenuButton.setFont(mFont);
    mMenuButton.setString("MENU");
    mMenuButton.setCharacterSize(40);
    mMenuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuBounds = mMenuButton.getLocalBounds();
    mMenuButton.setOrigin(menuBounds.left + menuBounds.width / 2.f,
                          menuBounds.top + menuBounds.height / 2.f);
    mMenuButton.setPosition(800.f / 2.f, 600.f / 2.f + 70.f);
}

// --- CÁC HÀM BÊN DƯỚI GIỮ NGUYÊN ---

// --- "ĐIỀU KHOẢN 1" (CLAUSE 1): PROCESSINPUT ---
void GameOverState::processInput(sf::Event &event)
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
void GameOverState::render(sf::RenderWindow &window)
{
    window.setView(window.getDefaultView());

    // 1. Vẽ Background Hình Ảnh (Vẽ đầu tiên để nó nằm dưới cùng)
    window.draw(mBackgroundSprite);

    // 2. Vẽ Lớp Phủ Mờ (Đè lên hình nền để làm tối hình nền đi một chút -> Nổi bật chữ)
    window.draw(mOverlay);

    // 3. Vẽ Chữ và Nút (Nằm trên cùng)
    window.draw(mGameOverText);
    window.draw(mRetryButton);
    window.draw(mMenuButton);
}

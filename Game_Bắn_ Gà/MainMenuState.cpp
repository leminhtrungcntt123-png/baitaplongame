#include "MainMenuState.h"
#include "Game.h"
#include "PlayingState.h" 
#include "DifficultyState.h"

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA MENU ---
MainMenuState::MainMenuState(Game* game) :
    // 1. "Kết nối" (Connect) "Hợp đồng" (Contract) "với" (with) "Nhạc trưởng" (Game)
    BaseState(game), 
    // 2. "Kết nối" (Connect) "các" (the) "Túi" (Pockets) "Texture" (Texture) "với" (with) "Tài Sản" (Assets) "toàn cục" (global) "của" (of) "Nhạc trưởng" (Game)
    mMenuBackgroundTexture(game->mMenuBackgroundTexture),
    mTitleTexture(game->mTitleTexture),
    mPlayButtonTexture(game->mPlayButtonTexture)
{
    // 3. "THI CÔNG" "SETUP" "SPRITE"
    // 3a. Setup Nền Menu
    mMenuBackgroundSprite.setTexture(mMenuBackgroundTexture);
    float scaleX = 800.f / mMenuBackgroundSprite.getLocalBounds().width;
    float scaleY = 600.f / mMenuBackgroundSprite.getLocalBounds().height;
    mMenuBackgroundSprite.setScale(scaleX, scaleY);

    // 3b. Setup Tiêu Đề (Logo)
    mTitleSprite.setTexture(mTitleTexture);
    mTitleSprite.setScale(0.8f, 0.8f);
    sf::FloatRect titleBounds = mTitleSprite.getLocalBounds();
    mTitleSprite.setOrigin(titleBounds.left + titleBounds.width / 2.f, 0.f);
    mTitleSprite.setPosition(400.f, 30.f);

    // 3c. Setup Nút "Play"
    mPlayButtonSprite.setTexture(mPlayButtonTexture);
    mPlayButtonSprite.setScale(1.0f, 1.0f);
    sf::FloatRect playBounds = mPlayButtonSprite.getLocalBounds();
    mPlayButtonSprite.setOrigin(playBounds.left + playBounds.width / 2.f,
                                playBounds.top + playBounds.height / 2.f);
    mPlayButtonSprite.setPosition(400.f, 300.f + 80.f);
}

void MainMenuState::processInput(sf::Event& event)
{
    // (Chỉ "lắng nghe" (listen for) "Click Chuột" (Mouse Click))
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // (Dùng "mGame->mWindow")
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

            // "Click" (Click) "trúng" (on) "nút" (button) "Play"?
            if (mPlayButtonSprite.getGlobalBounds().contains(mousePos))
            {
                mGame->changeState(std::make_unique<DifficultyState>(mGame));
            }
        }
    }
}

void MainMenuState::update(float deltaTime)
{
    // (Logic "Hover" (Hover) "của" (of) "bạn" (your))
    sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

    if (mPlayButtonSprite.getGlobalBounds().contains(mousePos))
    {
        mPlayButtonSprite.setScale(1.1f, 1.1f);
    }
    else
    {
        mPlayButtonSprite.setScale(1.0f, 1.0f);
    }
}

void MainMenuState::render(sf::RenderWindow& window)
{
    sf::View fixedView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(fixedView);
    // (Chúng ta "không" (do not) "cần" (need) "gọi" (to call) "window.clear()" "ở đây" (here), 
    //  "Nhạc trưởng" (Game) "sẽ" (will) "lo" (handle) "việc" (that) "đó" (that))

    window.draw(mMenuBackgroundSprite);
    window.draw(mTitleSprite);
    window.draw(mPlayButtonSprite);
}

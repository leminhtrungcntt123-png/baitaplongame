#include "PausedState.h"
#include "Game.h" // (Cần "Game" (Game) "để" (to) "lấy" (get) "Font" (Font) "và" (and) "Window" (Window))

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA PAUSE ---
PausedState::PausedState(Game* game) :
    BaseState(game),     // <-- ĐIỀU KHOẢN 1 (BẮT BUỘC)
    mFont(game->mFont)   // <-- ĐIỀU KHOẢN 2 (BẮT BUỘC - SỬA LỖI SỤP ĐỔ)
{
    // 3. "THI CÔNG" (IMPLEMENT) "SETUP" (SETUP) "UI" (UI) (Code "CẮT" (CUT) "từ" (from) "Game::Game()")

    // 3a. Setup Lớp Phủ Mờ
    mPauseOverlay.setSize(sf::Vector2f(800.f, 600.f));
    mPauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // 3b. Setup Nút "CHƠI TIẾP" (Resume)
    mPauseResumeButton.setFont(mFont);
    mPauseResumeButton.setString("CHOI TIEP");
    mPauseResumeButton.setCharacterSize(40);
    mPauseResumeButton.setFillColor(sf::Color::White);
    sf::FloatRect resumeBounds = mPauseResumeButton.getLocalBounds();
    mPauseResumeButton.setOrigin(resumeBounds.left + resumeBounds.width / 2.f,
                                 resumeBounds.top + resumeBounds.height / 2.f);
    mPauseResumeButton.setPosition(800.f / 2.f, 600.f / 2.f - 50.f);

    // 3c. Setup Nút "THOÁT" (Exit)
    mPauseExitButton.setFont(mFont);
    mPauseExitButton.setString("THOAT");
    mPauseExitButton.setCharacterSize(40);
    mPauseExitButton.setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = mPauseExitButton.getLocalBounds();
    mPauseExitButton.setOrigin(exitBounds.left + exitBounds.width / 2.f,
                               exitBounds.top + exitBounds.height / 2.f);
    mPauseExitButton.setPosition(800.f / 2.f, 600.f / 2.f + 50.f);
}


// --- "ĐIỀU KHOẢN 1" (CLAUSE 1): PROCESSINPUT ---
// (Code "CẮT" (CUT) "từ" (from) "Game::processInput")
void PausedState::processInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

            // "Click" (Click) "CHƠI TIẾP"?
            if (mPauseResumeButton.getGlobalBounds().contains(mousePos))
            {
                mGame->popState();
                return;
            }
            // "Click" (Click) "THOÁT"?
            if (mPauseExitButton.getGlobalBounds().contains(mousePos))
            {
                mGame->mWindow.close(); 
            }
        }
    }
}

void PausedState::update(float deltaTime)
{
    // (Logic "Hover" (Hover) "của" (of) "bạn" (your))
    sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

    // Hover "CHƠI TIẾP"
    if (mPauseResumeButton.getGlobalBounds().contains(mousePos))
        mPauseResumeButton.setFillColor(sf::Color::Yellow);
    else
        mPauseResumeButton.setFillColor(sf::Color::White);

    // Hover "THOÁT"
    if (mPauseExitButton.getGlobalBounds().contains(mousePos))
        mPauseExitButton.setFillColor(sf::Color::Yellow);
    else
        mPauseExitButton.setFillColor(sf::Color::White);
}

void PausedState::render(sf::RenderWindow& window)
{
    window.draw(mPauseOverlay);      // 1. Vẽ Lớp Phủ Mờ
    window.draw(mPauseResumeButton); // 2. Vẽ Nút "Chơi Tiếp"
    window.draw(mPauseExitButton);   // 3. Vẽ Nút "Thoát"
}

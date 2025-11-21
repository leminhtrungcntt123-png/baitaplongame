#include "../header/DifficultyState.h"
#include "../header/PlayingState.h"
#include "../header/MainMenuState.h"

DifficultyState::DifficultyState(Game* game) :
    BaseState(game),
    mBackgroundTexture(game->mSelectModeTexture), // Tận dụng ảnh nền Menu
    mFont(game->mFont)
{
    // 1. Setup Background (Chuẩn hóa 800x600)
    mBackgroundSprite.setTexture(mBackgroundTexture);
    float scaleX = 800.f / mBackgroundSprite.getLocalBounds().width;
    float scaleY = 600.f / mBackgroundSprite.getLocalBounds().height;
    mBackgroundSprite.setScale(scaleX, scaleY);
    mBackgroundSprite.setPosition(0.f, 0.f);

    // 2. Tiêu đề
    mTitleText.setFont(mFont);
    mTitleText.setString("SELECT MODEL");
    mTitleText.setCharacterSize(50);
    mTitleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin(titleBounds.width / 2.f, 0.f);
    mTitleText.setPosition(400.f, 100.f); // Giữa trên

    // 3. Nút EASY
    mEasyText.setFont(mFont);
    mEasyText.setString("EASY");
    mEasyText.setCharacterSize(40);
    mEasyText.setFillColor(sf::Color::Green);
    sf::FloatRect easyBounds = mEasyText.getLocalBounds();
    mEasyText.setOrigin(easyBounds.width / 2.f, 0.f);
    mEasyText.setPosition(400.f, 250.f);

    // 4. Nút NORMAL
    mNormalText.setFont(mFont);
    mNormalText.setString("NORMAL");
    mNormalText.setCharacterSize(40);
    mNormalText.setFillColor(sf::Color::Yellow);
    sf::FloatRect normBounds = mNormalText.getLocalBounds();
    mNormalText.setOrigin(normBounds.width / 2.f, 0.f);
    mNormalText.setPosition(400.f, 350.f);

    // 5. Nút HARD
    mHardText.setFont(mFont);
    mHardText.setString("ASIAN MODE");
    mHardText.setCharacterSize(40);
    mHardText.setFillColor(sf::Color::Red);
    sf::FloatRect hardBounds = mHardText.getLocalBounds();
    mHardText.setOrigin(hardBounds.width / 2.f, 0.f);
    mHardText.setPosition(400.f, 450.f);
}

void DifficultyState::processInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // Chuyển đổi chuột sang hệ tọa độ Game
            sf::Vector2i mousePixel = sf::Mouse::getPosition(mGame->mWindow);
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(mousePixel);

            // --- CHỌN EASY ---
            if (mEasyText.getGlobalBounds().contains(mousePos))
            {
                mGame->mDifficulty = Difficulty::Easy; // Lưu lựa chọn
                mGame->changeState(std::make_unique<PlayingState>(mGame)); // Vào chơi
            }
            // --- CHỌN NORMAL ---
            else if (mNormalText.getGlobalBounds().contains(mousePos))
            {
                mGame->mDifficulty = Difficulty::Normal;
                mGame->changeState(std::make_unique<PlayingState>(mGame));
            }
            // --- CHỌN HARD ---
            else if (mHardText.getGlobalBounds().contains(mousePos))
            {
                mGame->mDifficulty = Difficulty::Hard;
                mGame->changeState(std::make_unique<PlayingState>(mGame));
            }
        }
    }
}

void DifficultyState::update(float deltaTime)
{
    // 1. Lấy vị trí chuột (Hệ tọa độ Game)
    sf::Vector2i mousePixel = sf::Mouse::getPosition(mGame->mWindow);
    sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(mousePixel);

    // --- XỬ LÝ HIỆU ỨNG NÚT EASY ---
    if (mEasyText.getGlobalBounds().contains(mousePos))
    {
        mEasyText.setScale(1.2f, 1.2f); // Phóng to 20%
    }
    else
    {
        mEasyText.setScale(1.0f, 1.0f); // Trở về bình thường
    }

    // --- XỬ LÝ HIỆU ỨNG NÚT NORMAL ---
    if (mNormalText.getGlobalBounds().contains(mousePos))
    {
        mNormalText.setScale(1.2f, 1.2f);
    }
    else
    {
        mNormalText.setScale(1.0f, 1.0f);
    }

    // --- XỬ LÝ HIỆU ỨNG NÚT ASIAN MODE ---
    if (mHardText.getGlobalBounds().contains(mousePos))
    {
        mHardText.setScale(1.2f, 1.2f);
    }
    else
    {
        mHardText.setScale(1.0f, 1.0f);
    }
}

void DifficultyState::render(sf::RenderWindow& window)
{
    // Cưỡng chế Camera 800x600
    sf::View fixedView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(fixedView);

    window.draw(mBackgroundSprite);
    window.draw(mTitleText);
    window.draw(mEasyText);
    window.draw(mNormalText);
    window.draw(mHardText);
}

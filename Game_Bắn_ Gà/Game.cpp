#include "Game.h"
#include "MainMenuState.h"
#include "PlayingState.h" 
#include <iostream> 
#include <stdexcept>

Game::Game() :
    mWindow(sf::VideoMode(800, 600), "Game Ban Ga!")
{
    mWindow.setFramerateLimit(60);
    updateView(); 
    loadAssets(); 
    mShootSound.setBuffer(mShootBuffer);
    mExplosionSound.setBuffer(mExplosionBuffer);
    mPowerupSound.setBuffer(mPowerupBuffer);
    pushState(std::make_unique<MainMenuState>(this));

    // --- 3. KHỞI TẠO MẶC ĐỊNH ---
    mDifficulty = Difficulty::Normal;
}

// --- HÀM TẢI TÀI NGUYÊN (GIỮ NGUYÊN) ---
void Game::loadAssets()
{
    if (!mPlayerTexture.loadFromFile("player.png"))
        throw std::runtime_error("Error: Khong the tai file player.png!");
    if (!mBulletTexture.loadFromFile("bullet.png"))
        throw std::runtime_error("Error: Khong the tai file bullet.png!");
    if (!mEnemy1Texture.loadFromFile("enemy1.png"))
        throw std::runtime_error("Error: Khong the tai file enemy1.png!");
    if (!mEnemy3Texture.loadFromFile("enemy3.png"))
        throw std::runtime_error("Error: Khong the tai file enemy3.png!");
    if (!mPowerupTexture.loadFromFile("powerup.png"))
        throw std::runtime_error("Error: Khong the tai file powerup.png!");
    if (!mBackgroundTexture.loadFromFile("gameplay_background.png"))
        throw std::runtime_error("Error: Khong the tai file gameplay_background.png!");
    if (!mFont.loadFromFile("font.ttf"))
        throw std::runtime_error("Error: Khong the tai file font.ttf!");
    if (!mShootBuffer.loadFromFile("shoot.ogg"))
        throw std::runtime_error("Error: Khong the tai file shoot.ogg!");
    if (!mExplosionBuffer.loadFromFile("explosion.ogg"))
        throw std::runtime_error("Error: Khong the tai file explosion.ogg!");
    if (!mPowerupBuffer.loadFromFile("powerup.ogg"))
        throw std::runtime_error("Error: Khong the tai file powerup.ogg!");
    if (!mEnemyBulletTexture.loadFromFile("BulletEnemy2.png"))
        throw std::runtime_error("Error: Khong the tai file BulletEnemy2.png!");
    if (!mEnemy2Texture.loadFromFile("enemy2.png"))
        throw std::runtime_error("Error: Khong the tai file enemy2.png!");
    if (!mBossTexture.loadFromFile("boss.png"))
        throw std::runtime_error("Error: Khong the tai file boss.png!");
    if (!mBossBulletTexture.loadFromFile("BulletBoss.png"))
        throw std::runtime_error("Error: Khong the tai file BulletBoss.png!");
    if (!mMenuBackgroundTexture.loadFromFile("background.png"))
        throw std::runtime_error("Error: Khong the tai menu_background.png!");
    if (!mTitleTexture.loadFromFile("title.png"))
        throw std::runtime_error("Error: Khong the tai menu_title.png!");
    if (!mPlayButtonTexture.loadFromFile("play_button.png"))
        throw std::runtime_error("Error: Khong the tai menu_play_button.png!");
    if (!mPauseButtonTexture.loadFromFile("pause_button.png"))
        throw std::runtime_error("Error: Khong the tai file pause_button.png!");

    if (!mPlayerVIPTexture.loadFromFile("PlayerVIP.png"))
        throw std::runtime_error("Loi PlayerVIP.png!");

    if (!mBulletPlayerVIPTexture.loadFromFile("BulletPlayerVIP.png"))
        throw std::runtime_error("Loi BulletPlayerVIP.png!");

    if (!mItemChangePlayerTexture.loadFromFile("ItemChangePlayer.png"))
        throw std::runtime_error("Loi ItemChangePlayer.png!");

    if (!mMeteoriteTexture.loadFromFile("Meteorite.png"))
        throw std::runtime_error("Loi Meteorite.png!");

    if (!mItemBuffHPTexture.loadFromFile("ItemBuffHP.png"))
        throw std::runtime_error("Loi ItemBuffHP.png!");

    if (!mSelectModeTexture.loadFromFile("background_play.png"))
        throw std::runtime_error("Loi background_play.png!");
}

// --- HÀM "RUN" CHÍNH (GIỮ NGUYÊN) ---
void Game::run()
{
    while (mWindow.isOpen())
    {
        sf::Time dt = mDeltaClock.restart();
        float deltaTime = dt.asSeconds();

        // (Kiểm tra "nếu" (if) "không" (no) "còn" (remaining) "Chuyên gia" (Specialists) "nào" (whatsoever) "để" (to) "làm việc" (work), "thì" (then) "thoát" (exit))
        if (mStates.empty())
            mWindow.close();

        processInput();
        update(deltaTime);
        render();
    }
}

// --- "PHÒNG BAN" (DEPARTMENT) "INPUT" (INPUT) "SẠCH SẼ" (CLEAN) MỚI ---
void Game::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // 1. "Luôn" (Always) "kiểm tra" (check) "Nút X" (Close Button) "và" (and) "Resize" (Resize) "trước" (first)
        if (event.type == sf::Event::Closed)
            mWindow.close();
        else if (event.type == sf::Event::Resized)
        {
            updateView(); // (Gọi "Bản vá" (Patch) "Letterbox" (Letterbox))
        }
        else
        {
            // 2. "Ủy thác" (Delegate) "phần" (the) "còn lại" (rest) "cho" (to) "Chuyên gia" (Specialist) "hiện tại" (current)
            if (!mStates.empty())
                mStates.top()->processInput(event);
        }
    }
}

// --- "PHÒNG BAN" (DEPARTMENT) "UPDATE" (UPDATE) "SẠCH SẼ" (CLEAN) MỚI ---
void Game::update(float deltaTime)
{
    // "Ủy thác" (Delegate) "toàn bộ" (all) "công việc" (work) "cho" (to) "Chuyên gia" (Specialist) "hiện tại" (current)
    if (!mStates.empty())
    {
        mStates.top()->update(deltaTime);
    }
}

// --- "PHÒNG BAN" (DEPARTMENT) "RENDER" (RENDER) "SẠCH SẼ" (CLEAN) MỚI ---
void Game::render()
{
    mWindow.clear(); // 1. "Nhạc trưởng" (Game) "luôn" (always) "Xóa" (Clears) "Màn hình" (Screen)

    // 2. "Ủy thác" (Delegate) "toàn bộ" (all) "công việc" (work) "Vẽ" (Drawing) "cho" (to) "Chuyên gia" (Specialist) "hiện tại" (current)
    if (!mStates.empty())
    {
        mStates.top()->render(mWindow);
    }

    mWindow.display(); // 3. "Nhạc trưởng" (Game) "luôn" (always) "Hiển Thị" (Displays) "Màn hình" (Screen)
}

// --- "CÁC" (THE) "HÀM" (FUNCTIONS) "QUẢN LÝ" (MANAGER) "STATE" (STATE) "MỚI" (NEW) ---
void Game::pushState(std::unique_ptr<BaseState> state)
{
    mStates.push(std::move(state));
}

void Game::popState()
{
    if (!mStates.empty())
    {
        mStates.pop();
    }
}

void Game::changeState(std::unique_ptr<BaseState> state)
{
    // (Gỡ "bỏ" (pop) "state" (state) "cũ" (old) "TRƯỚC" (BEFORE), "sau đó" (then) "đẩy" (push) "state" (state) "mới" (new) "vào" (onto))
    popState();
    pushState(std::move(state));
}

BaseState* Game::getCurrentState()
{
    if (mStates.empty())
        return nullptr;
    return mStates.top().get();
}

// --- "HÀM" (FUNCTION) "LETTERBOX" (LETTERBOX) (GIỮ NGUYÊN) ---
void Game::updateView()
{
    const float DESIGN_WIDTH = 800.f;
    const float DESIGN_HEIGHT = 600.f;
    float designRatio = DESIGN_WIDTH / DESIGN_HEIGHT;
    float windowWidth = (float)mWindow.getSize().x;
    float windowHeight = (float)mWindow.getSize().y;
    float windowRatio = windowWidth / windowHeight;
    sf::View view(sf::FloatRect(0.f, 0.f, DESIGN_WIDTH, DESIGN_HEIGHT));
    float viewportX = 0.f;
    float viewportY = 0.f;
    float viewportWidth = 1.f;
    float viewportHeight = 1.f;
    if (windowRatio > designRatio)
    {
        viewportWidth = designRatio / windowRatio;
        viewportX = (1.f - viewportWidth) / 2.f;
    }
    else
    {
        viewportHeight = windowRatio / designRatio;
        viewportY = (1.f - viewportHeight) / 2.f;
    }
    view.setViewport(sf::FloatRect(viewportX, viewportY, viewportWidth, viewportHeight));
    mWindow.setView(view);
}

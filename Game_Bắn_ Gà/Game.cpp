#include "../header/Game.h"
#include "../header/MainMenuState.h"
#include "../header/PlayingState.h"
#include <iostream>
#include <stdexcept>

Game::Game() : mWindow(sf::VideoMode(800, 600), "COSMIC COMMANDER")
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

// --- HÀM TẢI TÀI NGUYÊN  ---
void Game::loadAssets()
{
    // Helper: try to load a texture, if it fails create a simple placeholder
    auto loadTextureOrPlaceholder = [&](sf::Texture &tex, const std::string &path, int w = 64, int h = 64)
    {
        if (!tex.loadFromFile(path))
        {
            std::cerr << "Warning: could not load '" << path << "' — using placeholder." << std::endl;
            sf::Image img;
            img.create(w, h, sf::Color::Magenta);
            tex.loadFromImage(img);
            return false;
        }
        return true;
    };

    // Try loading textures; missing ones become placeholders so app can run without assets
    loadTextureOrPlaceholder(mPlayerTexture, "model/player6.png",24, 24);
    loadTextureOrPlaceholder(mBulletTexture, "model/bullet.png", 8, 16);
    loadTextureOrPlaceholder(mEnemy1Texture, "model/ruoi.png", 48, 48);
    loadTextureOrPlaceholder(mEnemy3Texture, "model/gian.png", 48, 48);
    loadTextureOrPlaceholder(mPowerupTexture, "model/powerup.png", 24, 24);
    loadTextureOrPlaceholder(mBackgroundTexture, "model/backgound1.png", 800, 600);

    // Font: if missing, set flag and continue (texts will be guarded)
    if (!mFont.loadFromFile("model/font.ttf"))
    {
        std::cerr << "Warning: could not load font.ttf — text will be disabled." << std::endl;
        mFontLoaded = false;
    }
    else
    {
        mFontLoaded = true;
    }

    // Sounds: try to load, otherwise warn and continue
    if (!mShootBuffer.loadFromFile("model/shoot.ogg"))
    {
        std::cerr << "Warning: could not load shoot.ogg" << std::endl;
    }
    if (!mExplosionBuffer.loadFromFile("model/explosion.ogg"))
    {
        std::cerr << "Warning: could not load explosion.ogg" << std::endl;
    }
    if (!mPowerupBuffer.loadFromFile("model/powerup.ogg"))
    {
        std::cerr << "Warning: could not load powerup.ogg" << std::endl;
    }

    loadTextureOrPlaceholder(mEnemyBulletTexture, "model/BulletEnemy2.png", 8, 16);
    loadTextureOrPlaceholder(mEnemy2Texture, "model/ONG.png", 48, 48);
    loadTextureOrPlaceholder(mBossTexture, "model/boss.png", 128, 128);
    loadTextureOrPlaceholder(mBossBulletTexture, "model/BulletBoss.png", 6, 12);
    loadTextureOrPlaceholder(mMenuBackgroundTexture, "model/backgound3.png", 800, 600);
    loadTextureOrPlaceholder(mTitleTexture, "model/title.png", 400, 100);
    loadTextureOrPlaceholder(mPlayButtonTexture, "model/play_button.png", 160, 48);
    loadTextureOrPlaceholder(mPauseButtonTexture, "model/pause_button.png", 48, 48);
    loadTextureOrPlaceholder(mPlayerVIPTexture, "model/PlayerVIP2.png", 64, 64);
    loadTextureOrPlaceholder(mBulletPlayerVIPTexture, "model/BulletPlayerVIP.png", 8, 16);
    loadTextureOrPlaceholder(mItemChangePlayerTexture, "model/ItemChangePlayer.png", 32, 32);
    loadTextureOrPlaceholder(mMeteoriteTexture, "model/Meteorite.png", 64, 64);
    loadTextureOrPlaceholder(mItemBuffHPTexture, "model/ItemBuffHP.png", 24, 24);
    loadTextureOrPlaceholder(mSelectModeTexture, "model/backgound3.png", 800, 600);
    loadTextureOrPlaceholder(mLoseBackground, "model/backgoundlose.png", 800, 600);
    loadTextureOrPlaceholder(mWonBackground, "model/backgoundwin.png", 800, 600);
}

// --- HÀM "RUN" CHÍNH ---
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
        // 1. Luôn kiểm tra close button
        if (event.type == sf::Event::Closed)
            mWindow.close();
        else if (event.type == sf::Event::Resized)
        {
            updateView(); // (Gọi "Bản vá" (Patch) "Letterbox" (Letterbox))
        }
        else
        {
            // 2. Ủy thác phần còn lại chohamf chuyên dụng
            if (!mStates.empty())
                mStates.top()->processInput(event);
        }
    }
}

void Game::update(float deltaTime)
{
    // "Ủy thác" (Delegate) "toàn bộ" (all) "công việc" (work) "cho" (to) "Chuyên gia" (Specialist) "hiện tại" (current)
    if (!mStates.empty())
    {
        mStates.top()->update(deltaTime);
    }
}

void Game::render()
{
    mWindow.clear(); // 1. "Nhạc trưởng" Game luôn xóa màn hình

    // 2. Dùng hàm Render chuyên dụng để vẽ
    if (!mStates.empty())
    {
        mStates.top()->render(mWindow);
    }

    mWindow.display(); // 3. "Nhạc trưởng" Game luôn luôn hiển thị màn hình
}

// Các hàm quản lý State
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
    popState();
    pushState(std::move(state));
}

BaseState *Game::getCurrentState()
{
    if (mStates.empty())
        return nullptr;
    return mStates.top().get();
}

// --- Hàm Letterbox
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


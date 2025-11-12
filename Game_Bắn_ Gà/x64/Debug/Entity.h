#include "Game.h"
#include "CollisionManager.h"
#include "EnemyLvl1.h"
#include "EnemyLvl3.h"
#include "WaveManager.h"
#include "EnemyLvl2.h"
#include "Boss.h"
#include <iostream>  // Cho std::cerr
#include <stdexcept> // Cho std::runtime_error

// --- HÀM DỰNG (CONSTRUCTOR) ---
// Chạy 1 lần duy nhất khi "Game game;" được gọi
Game::Game()
// Khởi tạo cửa sổ
    : mWindow(sf::VideoMode(800, 600), "Game Ban Ga!"),
    // Khởi tạo các giá trị mặc định
    mBulletSpeed(800.f),
    mGameState(GameState::MainMenu),
    mWaveManager(mEnemies, mEnemy1Texture, mEnemy3Texture,
        mEnemy2Texture, mBossTexture, mBossBulletTexture),
    mScore(0)
{
    mWindow.setFramerateLimit(60);
    updateView();
    loadAssets();
    // --- KẾT NỐI ÂM THANH (THÊM VÀO ĐÂY) ---
    mShootSound.setBuffer(mShootBuffer);
    mExplosionSound.setBuffer(mExplosionBuffer);
    mPowerupSound.setBuffer(mPowerupBuffer);

    // --- THIẾT LẬP BACKGROUND (THÊM VÀO ĐÂY) ---
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Tự động co giãn ảnh nền để vừa khít cửa sổ 800x600
    mBackgroundSprite.setScale(
        (float)mWindow.getSize().x / mBackgroundSprite.getLocalBounds().width,
        (float)mWindow.getSize().y / mBackgroundSprite.getLocalBounds().height
    );

    // --- KHỞI TẠO PLAYER (THAY THẾ setupPlayer()) ---
    mPlayer = std::make_unique<Player>(mPlayerTexture, mBulletTexture, mPlayerBullets, mBulletSpeed);

    // Căn giữa tàu
    sf::FloatRect playerBounds = mPlayer->getBounds();
    float posX = (mWindow.getSize().x - playerBounds.width) / 2.f;
    float posY = mWindow.getSize().y - playerBounds.height - 50.f;
    mPlayer->setInitialPosition(posX, posY);
    
    // --- THIẾT LẬP UI (THÊM VÀO ĐÂY) ---
    mWinText.setFont(mFont);
    mWinText.setString("YOU WIN!");
    mWinText.setCharacterSize(64);
    mWinText.setFillColor(sf::Color::Yellow);

    // --- THÊM CODE SETUP "GAME OVER" ---
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(64);
    mGameOverText.setFillColor(sf::Color::Red);

    // Căn chữ ra giữa màn hình
    // Căn chữ "WIN" ra giữa màn hình
    sf::FloatRect winBounds = mWinText.getLocalBounds();
    mWinText.setOrigin(winBounds.left + winBounds.width / 2.f,
        winBounds.top + winBounds.height / 2.f);
    mWinText.setPosition(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);

    // Căn chữ "GAME OVER" ra giữa màn hình
    sf::FloatRect loseBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(loseBounds.left + loseBounds.width / 2.f,
        loseBounds.top + loseBounds.height / 2.f);
    mGameOverText.setPosition(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);

    // --- THÊM CODE SETUP "SPRITE" (ĐỒ HỌA) MENU MỚI ---

    // 1. Setup Nền Menu
    mMenuBackgroundSprite.setTexture(mMenuBackgroundTexture);
    // (Tự động co giãn "nền" (background) "mới" (new) vừa 800x600)
    mMenuBackgroundSprite.setScale(
        (float)mWindow.getSize().x / mMenuBackgroundSprite.getLocalBounds().width,
        (float)mWindow.getSize().y / mMenuBackgroundSprite.getLocalBounds().height
    );

    // 2. Setup Tiêu Đề (Logo)
    mTitleSprite.setTexture(mTitleTexture);
    mTitleSprite.setScale(0.8f, 0.8f);  // chỉnh size cho  title
    // (Căn giữa "Tiêu Đề" (Title) ở "trên" (top))
    sf::FloatRect titleBounds = mTitleSprite.getLocalBounds();
    mTitleSprite.setOrigin(titleBounds.left + titleBounds.width / 2.f, 0.f);
    mTitleSprite.setPosition(mWindow.getSize().x / 2.f, 30.f); // (Cách đỉnh 50px)

    // 3. Setup Nút "Play"
    mPlayButtonSprite.setTexture(mPlayButtonTexture);
    mPlayButtonSprite.setScale(1.0f, 1.0f); // (Chỉnh 0.5f cho vừa ý)
    // (Căn giữa "Nút Play" (Play Button) ở "giữa" (center) màn hình)
    sf::FloatRect playBounds = mPlayButtonSprite.getLocalBounds();
    mPlayButtonSprite.setOrigin(playBounds.left + playBounds.width / 2.f,
        playBounds.top + playBounds.height / 2.f);
    mPlayButtonSprite.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f + 80.f); // (Dịch xuống 50px)

    // Setup Thông Báo "VÒNG 2"
    mWaveNotifyText.setFont(mFont);
    mWaveNotifyText.setCharacterSize(64); // Cỡ chữ lớn
    mWaveNotifyText.setFillColor(sf::Color::White);
    mWaveNotifyText.setString("Vòng 2"); // (Sẽ được cập nhật)

    // Căn giữa
    sf::FloatRect waveBounds = mWaveNotifyText.getLocalBounds();
    mWaveNotifyText.setOrigin(waveBounds.left + waveBounds.width / 2.f,
        waveBounds.top + waveBounds.height / 2.f);
    mWaveNotifyText.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f); // Giữa màn hình

    // Setup Chữ "HP"
    mPlayerHPText.setFont(mFont);
    mPlayerHPText.setCharacterSize(24); // Cỡ chữ nhỏ
    mPlayerHPText.setFillColor(sf::Color::White);
    mPlayerHPText.setPosition(15.f, 15.f); // Góc trên bên trái
    // --- SETUP HP BAR (THÊM VÀO ĐÂY) ---
    float hpBarWidth = 150.f;  // (Chiều rộng tối đa 150px)
    float hpBarHeight = 20.f; // (Chiều cao 20px)

    // 1. Setup Nền (Background)
    mHpBarBackground.setSize(sf::Vector2f(hpBarWidth, hpBarHeight));
    mHpBarBackground.setFillColor(sf::Color(50, 50, 50)); // (Màu Xám Đen)
    mHpBarBackground.setOutlineColor(sf::Color::White);
    mHpBarBackground.setOutlineThickness(1.f);
    // (Đặt vị trí: Nằm bên phải chữ "HP", ví dụ: X = 60)
    mHpBarBackground.setPosition(60.f, 18.f);

    // 2. Setup Ruột (Foreground)
    mHpBarForeground.setSize(sf::Vector2f(hpBarWidth, hpBarHeight));
    mHpBarForeground.setFillColor(sf::Color::Green); // (Màu Xanh Lá)
    mHpBarForeground.setPosition(60.f, 18.f); // (Nằm y hệt Nền)

    // Setup Chữ "SCORE"
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setString("SCORE: 0");

    // Căn lề PHẢI (đặt ở góc trên bên phải)
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f); // Căn lề phải
    mScoreText.setPosition(mWindow.getSize().x - 15.f, 15.f); // Đặt ở góc (x-15, y=15)

    // --- SETUP LOGIC PAUSE (THÊM VÀO CUỐI HÀM DỰNG) ---

    // 1. Setup Nút Kích Hoạt (||)
   // 1. Setup Nút Kích Hoạt (||) [ĐÃ VÁ LẦN 2 - FIX LỖI CLICK]
    mPauseButtonSprite.setTexture(mPauseButtonTexture);
    mPauseButtonSprite.setScale(0.8f, 0.8f);

    // --- CHỈNH VỊ TRÍ (Góc Dưới-Trái) [FIX LỖI CLICK] ---
    // (Chúng ta KHÔNG DÙNG setOrigin() nữa)

    // (Lấy "kích thước đã scale" (scaled size) của nút SAU KHI setScale)
    sf::FloatRect pauseBounds = mPauseButtonSprite.getGlobalBounds();

    float padding = 20.f; // (Cách lề 20px)

    // "Đặt" (Set) "vị trí" (position) "Tọa độ X" (X-Coord)
    float pausePosX = padding;

    // "Đặt" (Set) "vị trí" (position) "Tọa độ Y" (Y-Coord)
    float pausePosY = (float)mWindow.getSize().y - padding - pauseBounds.height;

    // (SetPosition "này" (this) "sẽ" (will) "đặt" (set) "GÓC TRÊN-TRÁI" (TOP-LEFT) "của" (of) "nút" (button))
    mPauseButtonSprite.setPosition(pausePosX, pausePosY);
    // 2. Setup Lớp Phủ Mờ
    mPauseOverlay.setSize(sf::Vector2f(mWindow.getSize().x, mWindow.getSize().y));
    mPauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // (Màu đen, 150/255 độ mờ)
    // (Không cần setPosition, vì (0,0) là mặc định)

    // 3. Setup Nút "CHƠI TIẾP" (Resume)
    mPauseResumeButton.setFont(mFont);
    mPauseResumeButton.setString("CHOI TIEP");
    mPauseResumeButton.setCharacterSize(40);
    mPauseResumeButton.setFillColor(sf::Color::White);

    // (Căn giữa)
    sf::FloatRect resumeBounds = mPauseResumeButton.getLocalBounds();
    mPauseResumeButton.setOrigin(resumeBounds.left + resumeBounds.width / 2.f,
        resumeBounds.top + resumeBounds.height / 2.f);
    mPauseResumeButton.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f - 50.f);

    // 4. Setup Nút "THOÁT" (Exit)
    mPauseExitButton.setFont(mFont);
    mPauseExitButton.setString("THOAT");
    mPauseExitButton.setCharacterSize(40);
    mPauseExitButton.setFillColor(sf::Color::White);
    // (Căn giữa)
    sf::FloatRect exitBounds = mPauseExitButton.getLocalBounds();
    mPauseExitButton.setOrigin(exitBounds.left + exitBounds.width / 2.f,
        exitBounds.top + exitBounds.height / 2.f);
    mPauseExitButton.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f + 50.f);
}

// --- HÀM TẢI TÀI NGUYÊN ---
void Game::loadAssets()
{
    // Tất cả code "loadFromFile" của bạn được chuyển vào đây
    // Kỹ thuật mới: Nếu tải lỗi, "ném" ra 1 lỗi
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

    if (!mEnemyBulletTexture.loadFromFile("enemy_bullet.png"))
        throw std::runtime_error("Error: Khong the tai file enemy_bullet.png!");

    if (!mEnemy2Texture.loadFromFile("enemy2.png"))
        throw std::runtime_error("Error: Khong the tai file enemy2.png!");

    if (!mBossTexture.loadFromFile("boss.png"))
        throw std::runtime_error("Error: Khong the tai file boss.png!");

    if (!mBossBulletTexture.loadFromFile("BulletBoss.png"))
        throw std::runtime_error("Error: Khong the tai file BulletBoss.png!");

    if (!mMenuBackgroundTexture.loadFromFile("background.png")) // <-- Tên file nền MỚI của bạn
        throw std::runtime_error("Error: Khong the tai menu_background.png!");

    if (!mTitleTexture.loadFromFile("title.png")) // <-- Tên file tiêu đề MỚI của bạn
        throw std::runtime_error("Error: Khong the tai menu_title.png!");

    if (!mPlayButtonTexture.loadFromFile("play_button.png")) // <-- Tên file nút MỚI của bạn
        throw std::runtime_error("Error: Khong the tai menu_play_button.png!");

    if (!mPauseButtonTexture.loadFromFile("pause_button.png"))
        throw std::runtime_error("Error: Khong the tai file pause_button.png!");
}
// --- HÀM "RUN" CHÍNH ---
// Đây chính là Game Loop của bạn
void Game::run()
{
    while (mWindow.isOpen())
    {
        // 1. Tính Delta Time
        sf::Time dt = mDeltaClock.restart();
        float deltaTime = dt.asSeconds();

        // 2. Xử lý Input
        processInput();

        // 3. Cập nhật Logic
        update(deltaTime);

        // 4. Vẽ
        render();
    }
}

// --- THAY THẾ TOÀN BỘ HÀM PROCESSINPUT CŨ BẰNG HÀM NÀY ---
void Game::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        else if (event.type == sf::Event::Resized)
        {
            // "THI CÔNG" (EXECUTE) "LOGIC" (LOGIC) "LETTERBOXING" (LETTERBOXING)
            updateView();
        }
        // --- LOGIC MỚI: XỬ LÝ THEO TRẠNG THÁI ---

        // 1. Nếu đang ở MAIN MENU, chỉ lắng nghe click chuột
        if (mGameState == GameState::MainMenu)
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Lấy tọa độ chuột (chuyển đổi từ pixel cửa sổ sang tọa độ game)
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

                    // Kiểm tra xem có click trúng nút "START" không
                    if (mPlayButtonSprite.getGlobalBounds().contains(mousePos))
                    {
                        mGameState = GameState::Playing;
                    }
                }
            }
        }
        // 2. Nếu đang "PLAYING", chỉ lắng nghe bắn súng
        else if (mGameState == GameState::Playing)
        {
            mPlayer->handleInput(event);
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

                    // (Đây là logic "bắt click" bị thiếu)
                    if (mPauseButtonSprite.getGlobalBounds().contains(mousePos))
                    {
                        mGameState = GameState::Paused; // <-- CHUYỂN TRẠNG THÁI
                    }
                }
            }
        }
        else if (mGameState == GameState::Paused)
        {
            // "Bắt" (Catch) "click" (click) "để" (to) "Resume" (Resume) "hoặc" (or) "Exit" (Exit)
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

                    // Click "CHƠI TIẾP"?
                    if (mPauseResumeButton.getGlobalBounds().contains(mousePos))
                    {
                        mGameState = GameState::Playing; // <-- CHUYỂN TRẠNG THÁI
                    }
                    // Click "THOÁT"?
                    if (mPauseExitButton.getGlobalBounds().contains(mousePos))
                    {
                        mWindow.close(); // <-- ĐÓNG GAME
                    }
                }
            }
        }
    }
}

// --- PHÒNG BAN CẬP NHẬT LOGIC ---
void Game::update(float deltaTime)
{
    // 1. Chỉ kiểm tra "Hover" khi đang ở Main Menu
    if (mGameState == GameState::MainMenu)
    {
        // Lấy tọa độ chuột (chuyển đổi từ pixel cửa sổ sang tọa độ game)
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

        // Kiểm tra nút "START"
        if (mPlayButtonSprite.getGlobalBounds().contains(mousePos))
        {
            // "Phóng to" (Scale up) 1.1x khi "hover" (hover)
            mPlayButtonSprite.setScale(1.1f, 1.1f);
        }
        else
        {
            // "Về" (Reset) "kích thước" (scale) 1.0x "gốc" (original)
            mPlayButtonSprite.setScale(1.0f, 1.0f);
        }
    }

    // --- THÊM KHỐI "ELSE IF" NÀY VÀO ---
    else if (mGameState == GameState::Paused)
    {
        // "Thi công" (Implement) "Hover" (Hover) "cho" (for) "Menu Pause" (Pause Menu)
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

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
    // Chỉ cập nhật nếu đang "Playing"
    if (mGameState != GameState::Playing)
    {
        return; // Dừng ngay nếu Thắng/Thua
    }

    // 1. Cập nhật Tàu
    updatePlayer(deltaTime);
    updateHUD();

    // 2. Cập nhật logic bắn & AI
    updateBullets(deltaTime);
    mWaveManager.update(deltaTime);

    // 3. Cập nhật di chuyển (Quái, Nổ, Vật phẩm)
    updateEntities(deltaTime);

    // 4. Xử lý va chạm
    updateCollisions();

    // 5. Kiểm tra Thắng/Thua
    checkGameState();
}

// --- TRIỂN KHAI CÁC "PHÒNG BAN" MỚI ---

void Game::updatePlayer(float deltaTime)
{
    // (Dán code update Player)
    // Lấy "cờ báo hiệu" (shotFired) từ update
    bool shotFired = mPlayer->update(deltaTime, 800.f);
    if (shotFired)
    {
        mShootSound.play(); // Chơi âm thanh ở đây
    }
}

void Game::updateBullets(float deltaTime)
{
    // (Dán code update Đạn Tàu)
    for (int i = (int)mPlayerBullets.size() - 1; i >= 0; i--) {
        mPlayerBullets[i].update(deltaTime);
        if (mPlayerBullets[i].sprite.getPosition().y < 0) {
            mPlayerBullets.erase(mPlayerBullets.begin() + i);
        }
    }

    // (Dán code update Đạn Quái)
    for (int i = mEnemyBullets.size() - 1; i >= 0; i--) {
        mEnemyBullets[i].update(deltaTime);
        if (mEnemyBullets[i].sprite.getPosition().y > mWindow.getSize().y) {
            mEnemyBullets.erase(mEnemyBullets.begin() + i);
        }
    }
}

void Game::updateCollisions()
{
    // (Dán code gọi CollisionManager)
    CollisionManager::ProcessCollisions(mPlayerBullets, mEnemies);

    bool powerupCollected = CollisionManager::ProcessPlayerCollisions(*mPlayer, mPowerups);
    if (powerupCollected) {
        mPowerupSound.play();
    }

    // (Dùng code Bước 3 đã sửa)
    CollisionManager::ProcessPlayerHits(*mPlayer, mEnemies, mEnemyBullets);
}

void Game::updateEntities(float deltaTime)
{
    // (Dán code update Quái)
    for (int i = mEnemies.size() - 1; i >= 0; i--)
    {
        mEnemies[i]->update(deltaTime, mEnemyBullets, mEnemyBulletTexture, 800.f);

        if (mEnemies[i]->getPosition().y > mWindow.getSize().y) {
            mEnemies.erase(mEnemies.begin() + i);
        }
        else if (!mEnemies[i]->isAlive()) {
            sf::Vector2f deadPos = mEnemies[i]->getPosition();
            LootType drop = mEnemies[i]->dropType;
            mScore += mEnemies[i]->scoreValue; // Cộng điểm!
            mEnemies.erase(mEnemies.begin() + i);
            mExplosionSound.play();
            mExplosions.push_back(Explosion(deadPos));

            if (drop == LootType::UpgradeGun) {
                mPowerups.push_back(Powerup(mPowerupTexture, deadPos, Powerup::UpgradeGun));
            }
        }
    }

    // (Dán code update Vật phẩm)
    for (int i = mPowerups.size() - 1; i >= 0; i--) {
        mPowerups[i].update(deltaTime);
        if (mPowerups[i].sprite.getPosition().y > mWindow.getSize().y) {
            mPowerups.erase(mPowerups.begin() + i);
        }
    }

    // (Dán code update Vụ Nổ)
    for (int i = mExplosions.size() - 1; i >= 0; i--) {
        mExplosions[i].update(deltaTime);
        if (mExplosions[i].isFinished()) {
            mExplosions.erase(mExplosions.begin() + i);
        }
    }
}

void Game::checkGameState()
{
    // (Dán code kiểm tra Thắng/Thua)
    if (!mPlayer->isAlive()) {
        mGameState = GameState::GameOver;
    }
    else if (mWaveManager.isAllWavesFinished())
    {
        mGameState = GameState::Won;
    }
}

// --- PHÒNG BAN VẼ ---
void Game::render()
{
    // 1. Xóa màn hình (Luôn luôn làm)
    mWindow.clear(); // (Ghi chú: Xóa clear() ở chỗ khác nếu có)

    // 2. Vẽ Background (Luôn luôn làm)
    mWindow.draw(mBackgroundSprite);

    // --- 3. LOGIC VẼ THEO TRẠNG THÁI ---

    if (mGameState == GameState::MainMenu)
    {
        // --- VẼ GIAO DIỆN MENU (Đồ Họa Mới) ---
        mWindow.draw(mMenuBackgroundSprite); // (Vẽ "nền" (background) MENU)
        mWindow.draw(mTitleSprite);
        mWindow.draw(mPlayButtonSprite);
    }
    else
    {
        // --- VẼ GIAO DIỆN GAME (Playing, Won, GameOver) ---
        mWindow.draw(mBackgroundSprite); // (Vẽ "nền" (background) GAMEPLAY)
        // Vẽ Kẻ thù
        for (auto& enemy : mEnemies) {
            enemy->draw(mWindow);
        }
        // Vẽ Vụ Nổ
        for (auto& explosion : mExplosions) {
            explosion.draw(mWindow);
        }
        // Vẽ Vật phẩm
        for (auto& powerup : mPowerups) {
            powerup.draw(mWindow);
        }
        // Vẽ Tàu
        mPlayer->draw(mWindow);
        // Vẽ Đạn Tàu
        for (auto& bullet : mPlayerBullets) {
            bullet.draw(mWindow);
        }
        // Vẽ Đạn Quái
        for (auto& bullet : mEnemyBullets) {
            bullet.draw(mWindow);
        }

        // --- VẼ HUD (Thứ tự rất quan trọng) ---
        mWindow.draw(mPlayerHPText); // (Vẽ chữ "HP")

        // (Vẽ Nền TRƯỚC)
        mWindow.draw(mHpBarBackground);
        // (Vẽ Ruột SAU (đè lên Nền))
        mWindow.draw(mHpBarForeground);

        // (Vẽ nó TRƯỚC UI Thắng/Thua)
        mWindow.draw(mPlayerHPText);
        // Điểm
        mWindow.draw(mScoreText);
        // --- THÊM DÒNG NÀY (VẼ NÚT PAUSE "||") ---
        mWindow.draw(mPauseButtonSprite);

        // --- LOGIC VẼ UI (Thắng/Thua/Khoảng nghỉ) ---
        if (mGameState == GameState::Won) {
            // 1. Vẽ BÓNG (Đen, dịch 3px)
            mWinText.setFillColor(sf::Color::Black);
            mWinText.setPosition(800.f / 2.f + 3.f, 600.f / 2.f + 3.f);
            mWindow.draw(mWinText);
            // 2. Vẽ CHỮ (Vàng, vị trí cũ)
            mWinText.setFillColor(sf::Color::Yellow);
            mWinText.setPosition(800.f / 2.f, 600.f / 2.f);
            mWindow.draw(mWinText);
        }
        else if (mGameState == GameState::GameOver) {
            // 1. Vẽ BÓNG (Đen, dịch 3px)
            mGameOverText.setFillColor(sf::Color::Black);
            mGameOverText.setPosition(800.f / 2.f + 3.f, 600.f / 2.f + 3.f);
            mWindow.draw(mGameOverText);

            // 2. Vẽ CHỮ (Đỏ, vị trí cũ)
            mGameOverText.setFillColor(sf::Color::Red);
            mGameOverText.setPosition(800.f / 2.f, 600.f / 2.f);
            mWindow.draw(mGameOverText);
        }
        // --- THÊM KHỐI "ELSE IF" NÀY VÀO ---
        else if (mWaveManager.getState() == WaveManager::State::Intermission)
        {
            // Lấy số Wave tiếp theo (ví dụ: "VONG 2")
            std::string waveStr = "Vòng " + std::to_string(mWaveManager.getNextWaveNumber());
            mWaveNotifyText.setString(waveStr);

            // Căn giữa lại (vì chữ "VONG 10" rộng hơn "VONG 2")
            sf::FloatRect waveBounds = mWaveNotifyText.getLocalBounds();
            mWaveNotifyText.setOrigin(waveBounds.left + waveBounds.width / 2.f,
                waveBounds.top + waveBounds.height / 2.f);

            mWindow.draw(mWaveNotifyText);
        }
        if (mGameState == GameState::Paused)
        {
            mWindow.draw(mPauseOverlay);      // 1. Vẽ Lớp Phủ Mờ
            mWindow.draw(mPauseResumeButton); // 2. Vẽ Nút "Chơi Tiếp"
            mWindow.draw(mPauseExitButton);   // 3. Vẽ Nút "Thoát"
        }
    }
    mWindow.display();
}
// --- THÊM HÀM MỚI NÀY VÀO CUỐI FILE ---
void Game::updateHUD()
{
    // --- "VÁ" (PATCH) LOGIC HP ---
    int currentHp = mPlayer->getHP();
    int maxHp = 10; // (Hardcode "HP Tối Đa" của Player)

    // 1. Cập nhật Chữ (Text)
    // (Chỉ hiện "HP", không hiện số, thanh bar sẽ lo)
    mPlayerHPText.setString("HP");

    // 2. Cập nhật Thanh (Bar)
    // (Tính toán % máu còn lại)
    float hpPercent = (float)currentHp / (float)maxHp;
    if (hpPercent < 0) hpPercent = 0; // (Không cho âm)

    // (Lấy chiều rộng "tối đa" của thanh bar)
    float fullBarWidth = mHpBarBackground.getSize().x;

    // (Cập nhật "chiều rộng" (width) của "ruột" (foreground))
    mHpBarForeground.setSize(sf::Vector2f(fullBarWidth * hpPercent,
        mHpBarForeground.getSize().y));

    // (Tùy chọn: Đổi màu khi máu thấp)
    if (hpPercent < 0.3f) // (Dưới 30% máu)
        mHpBarForeground.setFillColor(sf::Color::Red);
    else if (hpPercent < 0.6f) // (Dưới 60% máu)
        mHpBarForeground.setFillColor(sf::Color::Yellow);
    else
        mHpBarForeground.setFillColor(sf::Color::Green);


    // --- Cập nhật Score (Giữ nguyên) ---
    mScoreText.setString("SCORE: " + std::to_string(mScore));

    // (Code căn lề phải CŨ... giữ nguyên)
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f);
    mScoreText.setPosition(mWindow.getSize().x - 15.f, 15.f);
}
void Game::updateView()
{
    // "Tỷ lệ" (Aspect Ratio) "Thiết Kế" (Design) (Game của chúng ta là 800x600, tức là 4:3)
    const float DESIGN_WIDTH = 800.f;
    const float DESIGN_HEIGHT = 600.f;
    float designRatio = DESIGN_WIDTH / DESIGN_HEIGHT;

    // "Kích thước" (Size) "Cửa Sổ" (Window) "Hiện Tại" (Current) (ví dụ: 1920x1080)
    float windowWidth = (float)mWindow.getSize().x;
    float windowHeight = (float)mWindow.getSize().y;
    float windowRatio = windowWidth / windowHeight;

    // "Tạo" (Create) "Camera" (View) "Logic" (Logical) (luôn là 800x600)
    // "Camera" (View) "này" (this) "báo" (tells) "cho" (the) "game" (game) "biết" (that) "thế giới" (world) "chỉ" (is only) "rộng" (wide) "800px" (800px)
    sf::View view(sf::FloatRect(0.f, 0.f, DESIGN_WIDTH, DESIGN_HEIGHT));

    float viewportX = 0.f;
    float viewportY = 0.f;
    float viewportWidth = 1.f;
    float viewportHeight = 1.f;

    // "So sánh" (Compare) "tỷ lệ" (ratios)
    if (windowRatio > designRatio)
    {
        // (Cửa sổ "RỘNG" (WIDER) hơn 4:3 -> Thêm "viền đen" (black bars) "hai bên" (sides))
        // (Ví dụ: 16:9 > 4:3)
        viewportWidth = designRatio / windowRatio;
        viewportX = (1.f - viewportWidth) / 2.f;
    }
    else // (windowRatio <= designRatio)
    {
        // (Cửa sổ "CAO" (TALLER) hơn 4:3 -> Thêm "viền đen" (black bars) "trên dưới" (top/bottom))
        viewportHeight = windowRatio / designRatio;
        viewportY = (1.f - viewportHeight) / 2.f;
    }

    // "Thiết lập" (Set) "cái" (the) "khung" (viewport) "viền đen" (black bar)
    // "Viewport" (Viewport) "này" (this) "báo" (tells) "SFML" (SFML) "chỉ" (to only) "vẽ" (draw) "ở" (in) "khu vực" (area) "giữa" (center) "này" (this)
    view.setViewport(sf::FloatRect(viewportX, viewportY, viewportWidth, viewportHeight));

    // "Áp dụng" (Apply) "Camera" (View) "tổng hợp" (combined) "này" (this)
    mWindow.setView(view);
}
   

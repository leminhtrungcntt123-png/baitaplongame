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
    mWinText.setString("CHIEN THANG!");
    mWinText.setCharacterSize(64);
    mWinText.setFillColor(sf::Color::Yellow);

    // --- THÊM CODE SETUP "GAME OVER" ---
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(64);
    mGameOverText.setFillColor(sf::Color::Red);

    // Căn chữ ra giữa màn hình
    sf::FloatRect textBounds = mWinText.getLocalBounds();
    mWinText.setOrigin(textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);
    mWinText.setPosition(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);

    // --- THÊM CÁC DÒNG NÀY VÀO CUỐI HÀM DỰNG ---

// Setup Nút "START GAME"
    mStartButton.setFont(mFont);
    mStartButton.setString("START GAME");
    mStartButton.setCharacterSize(40); // Cỡ chữ 40
    mStartButton.setFillColor(sf::Color::White);

    // Căn giữa nút "START"
    sf::FloatRect startBounds = mStartButton.getLocalBounds();
    mStartButton.setOrigin(startBounds.left + startBounds.width / 2.f,
        startBounds.top + startBounds.height / 2.f);
    mStartButton.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f - 50.f); // Đặt ở giữa, dịch lên 50

    // Setup Nút "EXIT"
    mExitButton.setFont(mFont);
    mExitButton.setString("EXIT");
    mExitButton.setCharacterSize(40);
    mExitButton.setFillColor(sf::Color::White);

    // Căn giữa nút "EXIT"
    sf::FloatRect exitBounds = mExitButton.getLocalBounds();
    mExitButton.setOrigin(exitBounds.left + exitBounds.width / 2.f,
        exitBounds.top + exitBounds.height / 2.f);
    mExitButton.setPosition(mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f + 50.f); // Đặt ở giữa, dịch xuống 50

    // --- THÊM CODE SETUP NÀY VÀO ---

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
    mPlayerHPText.setString("HP: 10");

    // Setup Chữ "SCORE"
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setString("SCORE: 0");

    // Căn lề PHẢI (đặt ở góc trên bên phải)
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f); // Căn lề phải
    mScoreText.setPosition(mWindow.getSize().x - 15.f, 15.f); // Đặt ở góc (x-15, y=15)
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

    if (!mBackgroundTexture.loadFromFile("background.png"))
        throw std::runtime_error("Error: Khong the tai file background.png!");

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
                    if (mStartButton.getGlobalBounds().contains(mousePos))
                    {
                        mGameState = GameState::Playing; // Chuyển trạng thái
                    }
                    // Kiểm tra xem có click trúng nút "EXIT" không
                    if (mExitButton.getGlobalBounds().contains(mousePos))
                    {
                        mWindow.close(); // Thoát game
                    }
                }
            }
        }
        // 2. Nếu đang "PLAYING", chỉ lắng nghe bắn súng
        else if (mGameState == GameState::Playing)
        {
            mPlayer->handleInput(event);
        }
    }
}

// --- PHÒNG BAN CẬP NHẬT LOGIC ---
void Game::update(float deltaTime)
{
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
    bool shotFired = mPlayer->update(deltaTime, static_cast<float>(mWindow.getSize().x));
    if (shotFired)
    {
        mShootSound.play(); // Chơi âm thanh ở đây
    }
}

void Game::updateBullets(float deltaTime)
{
    // (Dán code update Đạn Tàu)
    for (int i = mPlayerBullets.size() - 1; i >= 0; i--) {
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
        mEnemies[i]->update(deltaTime, mEnemyBullets, mEnemyBulletTexture, mWindow.getSize().x);

        if (mEnemies[i]->sprite.getPosition().y > mWindow.getSize().y) {
            mEnemies.erase(mEnemies.begin() + i);
        }
        else if (!mEnemies[i]->isAlive()) {
            sf::Vector2f deadPos = mEnemies[i]->sprite.getPosition();
            EnemyBase::LootType drop = mEnemies[i]->dropType;
            mScore += mEnemies[i]->scoreValue; // Cộng điểm!
            mEnemies.erase(mEnemies.begin() + i);
            mExplosionSound.play();
            mExplosions.push_back(Explosion(deadPos));

            if (drop == EnemyBase::UpgradeGun) {
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
        // --- VẼ GIAO DIỆN MENU ---
        mWindow.draw(mStartButton);
        mWindow.draw(mExitButton);
    }
    else
    {
        // --- VẼ GIAO DIỆN GAME (Playing, Won, GameOver) ---

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
        // (Vẽ nó TRƯỚC UI Thắng/Thua)
        mWindow.draw(mPlayerHPText);
        mWindow.draw(mScoreText);

        // --- SỬA LOGIC VẼ UI (Thắng/Thua/Khoảng nghỉ) ---
        if (mGameState == GameState::Won) {
            mWindow.draw(mWinText);
        }
        else if (mGameState == GameState::GameOver) {
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
    }
    mWindow.display();
}
// --- THÊM HÀM MỚI NÀY VÀO CUỐI FILE ---
void Game::updateHUD()
{
    // Cập nhật HP (Code cũ từ updatePlayer)
    mPlayerHPText.setString("HP: " + std::to_string(mPlayer->getHP()));

    // Cập nhật Score
    mScoreText.setString("SCORE: " + std::to_string(mScore));

    // Căn lề PHẢI lại cho Score (Vì "SCORE: 1000" rộng hơn "SCORE: 0")
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f);
    mScoreText.setPosition(mWindow.getSize().x - 15.f, 15.f);
}
   
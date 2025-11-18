#include "PlayingState.h"
#include "Game.h"
#include "PausedState.h"
#include <string> 
#include "GameOverState.h" 
#include "WonState.h"      // <-- THÊM DÒNG NÀY

// --- HÀM DỰNG (CONSTRUCTOR) CỦA CHUYÊN GIA GAMEPLAY ---
PlayingState::PlayingState(Game* game) :
    BaseState(game), // 1. "Kết nối" (Connect) "Hợp đồng" (Contract)

    // 2. "Kết nối" (Connect) "Tài Sản" (Assets)
    mBackgroundTexture(game->mBackgroundTexture),
    mPauseButtonTexture(game->mPauseButtonTexture),
    mEnemyBulletTexture(game->mEnemyBulletTexture),
    mPowerupTexture(game->mPowerupTexture),
    mFont(game->mFont),

    // 3. "Khởi tạo" (Initialize) "các" (the) "Hệ thống" (Systems) "con" (sub-systems)
    // (Lưu ý: truyền các tài sản toàn cục của Game vào "WaveManager")
    mWaveManager(mEnemies, game->mEnemy1Texture, game->mEnemy3Texture,
        game->mEnemy2Texture, game->mBossTexture, game->mBossBulletTexture,
        game->mMeteoriteTexture),
    mBulletSpeed(800.f),
    mScore(0),
    mKillCount(0),
    mPlayState(PlayState::Playing) // (Bắt đầu "ở" (at) "trạng thái" (state) "Playing" (Playing))
{
    // --- THÊM ĐOẠN NÀY ĐỂ TRUYỀN ĐỘ KHÓ ---
    if (game->mDifficulty == Difficulty::Easy)
        mWaveManager.setDifficulty(0);
    else if (game->mDifficulty == Difficulty::Normal)
        mWaveManager.setDifficulty(1);
    else
        mWaveManager.setDifficulty(2);

    // --- 4. THI CÔNG SETUP 
    // 4a. Setup Background
    mBackgroundSprite.setTexture(mBackgroundTexture);
    mBackgroundSprite.setScale(
        800.f / mBackgroundSprite.getLocalBounds().width,
        600.f / mBackgroundSprite.getLocalBounds().height
    );

    // 4b. Setup Player
    // (Dùng "Tài Sản" (Assets) "toàn cục" (global) "của" (of) "Game" (Game))
    mPlayer = std::make_unique<Player>(game->mPlayerTexture, game->mBulletTexture,
        mPlayerBullets, mBulletSpeed,
        game->mPlayerVIPTexture,      // <-- THÊM DÒNG NÀY
        game->mBulletPlayerVIPTexture // <-- THÊM DÒNG NÀY
    );
    sf::FloatRect playerBounds = mPlayer->getBounds();
    float posX = (800.f - playerBounds.width) / 2.f;
    float posY = 600.f - playerBounds.height - 50.f;
    mPlayer->setInitialPosition(posX, posY);

    // 4c. Setup UI (Win/Lose)
    mWinText.setFont(mFont);
    mWinText.setString("YOU WIN!");
    mWinText.setCharacterSize(64);
    mWinText.setFillColor(sf::Color::Yellow);
    sf::FloatRect winBounds = mWinText.getLocalBounds();
    mWinText.setOrigin(winBounds.left + winBounds.width / 2.f, winBounds.top + winBounds.height / 2.f);
    mWinText.setPosition(800.f / 2.f, 600.f / 2.f);

    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(64);
    mGameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect loseBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(loseBounds.left + loseBounds.width / 2.f, loseBounds.top + loseBounds.height / 2.f);
    mGameOverText.setPosition(800.f / 2.f, 600.f / 2.f);

    // 4d. Setup UI (HUD)
    mWaveNotifyText.setFont(mFont);
    mWaveNotifyText.setCharacterSize(64);
    mWaveNotifyText.setFillColor(sf::Color::White);
    mWaveNotifyText.setString("Vòng 2"); // (Tạm)
    sf::FloatRect waveBounds = mWaveNotifyText.getLocalBounds();
    mWaveNotifyText.setOrigin(waveBounds.left + waveBounds.width / 2.f, waveBounds.top + waveBounds.height / 2.f);
    mWaveNotifyText.setPosition(800.f / 2.f, 600.f / 2.f);

    mPlayerHPText.setFont(mFont);
    mPlayerHPText.setCharacterSize(24);
    mPlayerHPText.setFillColor(sf::Color::White);
    mPlayerHPText.setPosition(15.f, 15.f);

    float hpBarWidth = 150.f;
    float hpBarHeight = 20.f;
    mHpBarBackground.setSize(sf::Vector2f(hpBarWidth, hpBarHeight));
    mHpBarBackground.setFillColor(sf::Color(50, 50, 50));
    mHpBarBackground.setOutlineColor(sf::Color::White);
    mHpBarBackground.setOutlineThickness(1.f);
    mHpBarBackground.setPosition(60.f, 18.f);
    mHpBarForeground.setSize(sf::Vector2f(hpBarWidth, hpBarHeight));
    mHpBarForeground.setFillColor(sf::Color::Green);
    mHpBarForeground.setPosition(60.f, 18.f);

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setString("SCORE: 0");
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f);
    mScoreText.setPosition(800.f - 15.f, 15.f);

    // --- THÊM SETUP ĐỒNG HỒ ---
    mTimerText.setFont(mFont);
    mTimerText.setCharacterSize(24);
    mTimerText.setFillColor(sf::Color::Cyan); // Màu xanh lơ cho nổi bật
    mTimerText.setString("TIME: 00");
    sf::FloatRect timeBounds = mTimerText.getLocalBounds();
    mTimerText.setOrigin(timeBounds.left + timeBounds.width, 0.f);
    // Đặt vị trí: Bên trái điểm số một chút, hoặc ngay dưới điểm số
    mTimerText.setPosition(800.f - 15.f, 40.f);

    // 4e. Setup UI (Boss)
    mBossHpBarBackground.setFillColor(sf::Color(50, 50, 50));
    mBossHpBarBackground.setOutlineColor(sf::Color::White);
    mBossHpBarBackground.setOutlineThickness(1.f);
    mBossHpBarForeground.setFillColor(sf::Color::Red);

    // 4f. Setup UI (Nút Pause)
    mPauseButtonSprite.setTexture(mPauseButtonTexture);
    mPauseButtonSprite.setScale(0.8f, 0.8f);
    sf::FloatRect pauseBounds = mPauseButtonSprite.getGlobalBounds();
    float padding = 20.f;
    float pausePosX = padding;
    float pausePosY = 600.f - padding - pauseBounds.height;
    mPauseButtonSprite.setPosition(pausePosX, pausePosY);
}

void PlayingState::processInput(sf::Event& event)
{
    // (Nếu "trạng thái" (state) "Play" (Play) "KHÔNG PHẢI" (IS NOT) "là" (is) "Playing" (Playing), "ví dụ" (e.g.) "là" (is) "GameOver" (GameOver), 
    //  "thì" (then) "KHÔNG" (DO NOT) "nhận" (process) "input" (input) "nữa" (anymore))
    if (mPlayState != PlayState::Playing)
        return;

    // 1. "Ủy thác" (Delegate) "Input" (Input) "cho" (to) "Player" (Player)
    mPlayer->handleInput(event);

    // 2. "Kiểm tra" (Check) "click" (click) "vào" (on) "nút" (button) "Pause" (Pause)
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = mGame->mWindow.mapPixelToCoords(sf::Mouse::getPosition(mGame->mWindow));

            if (mPauseButtonSprite.getGlobalBounds().contains(mousePos))
            {
                mGame->pushState(std::make_unique<PausedState>(mGame));
            }
        }
    }
}


// --- "ĐIỀU KHOẢN 2" (CLAUSE 2): UPDATE ---
// (Code "CẮT" (CUT) "từ" (from) "Game::update")
void PlayingState::update(float deltaTime)
{
    // (Nếu "trạng thái" (state) "Play" (Play) "KHÔNG PHẢI" (IS NOT) "là" (is) "Playing" (Playing), "ví dụ" (e.g.) "là" (is) "GameOver" (GameOver),
    //  "thì" (then) "KHÔNG" (DO NOT) "cập nhật" (update) "logic" (logic) "nữa" (anymore))
    if (mPlayState != PlayState::Playing)
        return;

    // (Đây "là" (is) "toàn bộ" (the entire) "logic" (logic) "update" (update) "gameplay" (gameplay) "CŨ" (OLD) "của" (of) "bạn" (your))
    updatePlayer(deltaTime);
    updateHUD(); // (Phải "update" (update) "HUD" (HUD) "TRƯỚC" (BEFORE) "updateEntities" (updateEntities) "để" (to) "cập nhật" (update) "HP Boss" (Boss HP))
    updateBullets(deltaTime);
    mWaveManager.update(deltaTime);
    updateEntities(deltaTime);
    updateCollisions();
    checkGameState();
}

void PlayingState::updatePlayer(float deltaTime)
{
    // (Dùng "Âm thanh" (Sound) "toàn cục" (global) "của" (of) "Game" (Game))
    bool shotFired = mPlayer->update(deltaTime, 800.f);
    if (shotFired)
    {
        mGame->mShootSound.play(); // (Sửa: Thêm "mGame->")
    }
}

void PlayingState::updateBullets(float deltaTime)
{
    for (int i = (int)mPlayerBullets.size() - 1; i >= 0; i--) {
        mPlayerBullets[i].update(deltaTime);
        if (mPlayerBullets[i].sprite.getPosition().y < 0) {
            mPlayerBullets.erase(mPlayerBullets.begin() + i);
        }
    }
    for (int i = (int)mEnemyBullets.size() - 1; i >= 0; i--) {
        mEnemyBullets[i].update(deltaTime);
        if (mEnemyBullets[i].sprite.getPosition().y > 600.f) { // (Sửa: Dùng 600.f)
            mEnemyBullets.erase(mEnemyBullets.begin() + i);
        }
    }
}

void PlayingState::updateCollisions()
{
    CollisionManager::ProcessCollisions(mPlayerBullets, mEnemies);

    bool powerupCollected = CollisionManager::ProcessPlayerCollisions(*mPlayer, mPowerups);
    if (powerupCollected) {
        mGame->mPowerupSound.play(); // (Sửa: Thêm "mGame->")
    }
    CollisionManager::ProcessPlayerHits(*mPlayer, mEnemies, mEnemyBullets);
}

void PlayingState::updateEntities(float deltaTime)
{
    for (int i = (int)mEnemies.size() - 1; i >= 0; i--)
    {
        // (Dùng 800.f)
        mEnemies[i]->update(deltaTime, mEnemyBullets, mEnemyBulletTexture, 800.f);

        if (mEnemies[i]->getPosition().y > 600.f) { // (Sửa: Dùng 600.f)
            mEnemies.erase(mEnemies.begin() + i);
        }
        else if (!mEnemies[i]->isAlive())
        {
            sf::Vector2f deadPos = mEnemies[i]->getPosition();
            LootType drop = mEnemies[i]->dropType;
            mScore += mEnemies[i]->scoreValue;
            mEnemies.erase(mEnemies.begin() + i);

            mGame->mExplosionSound.play();
            mExplosions.push_back(Explosion(deadPos));

            // --- [HỆ THỐNG RƠI ĐỒ THÔNG MINH] ---
            mKillCount++; // 1. Tăng biến đếm

            // 2. Quy tắc: Cứ mỗi 15 kill -> Rơi Máu
            if (mKillCount >= 15)
            {
                // Tạo item Máu (Dùng texture từ Game)
                mPowerups.push_back(Powerup(mGame->mItemBuffHPTexture, deadPos, Powerup::HealthPack));

                mKillCount = 0; // Reset đếm về 0
            }
            // 3. Nếu chưa đủ 15 kill -> Rơi random theo cơ chế cũ (Súng/Biến hình)
            else
            {
                if (drop == LootType::UpgradeGun) {
                    mPowerups.push_back(Powerup(mPowerupTexture, deadPos, Powerup::UpgradeGun));
                }
                else if (drop == LootType::MegaUpgrade) {
                    mPowerups.push_back(Powerup(mGame->mItemChangePlayerTexture, deadPos, Powerup::MegaUpgrade));
                }
            }
        }
    }

    for (int i = (int)mPowerups.size() - 1; i >= 0; i--) {
        mPowerups[i].update(deltaTime);
        if (mPowerups[i].sprite.getPosition().y > 600.f) { // (Sửa: Dùng 600.f)
            mPowerups.erase(mPowerups.begin() + i);
        }
    }
    for (int i = (int)mExplosions.size() - 1; i >= 0; i--) {
        mExplosions[i].update(deltaTime);
        if (mExplosions[i].isFinished()) {
            mExplosions.erase(mExplosions.begin() + i);
        }
    }
}

void PlayingState::checkGameState()
{
    // (Kiểm tra "nếu" (if) "trạng thái" (state) "hiện tại" (current) "vẫn" (is still) "là" (is) "Playing" (Playing) "để" (to) "tránh" (avoid) "gọi" (calling) "lệnh" (command) "nhiều" (multiple) "lần" (times))
    if (mPlayState == PlayState::Playing)
    {
        // 1. Thua vì chết
        if (!mPlayer->isAlive())
        {
            // "BÁO CÁO": "Sa thải" (Fire) "tôi" (me) "(PlayingState)" (PlayingState) "và" (and) "thuê" (hire) "Chuyên gia Game Over" (GameOver Specialist)
            mGame->changeState(std::make_unique<GameOverState>(mGame));
        }
        // 2. Thắng toàn bộ Game
        else if (mWaveManager.isAllWavesFinished())
        {
            mGame->changeState(std::make_unique<WonState>(mGame));
        }
        // 3. KIỂM TRA THỜI GIAN (LUẬT MỚI)
        else
        {
            float timeLeft = mWaveManager.getRemainingTime();
            int currentType = mWaveManager.getCurrentEnemyType();

            // Nếu hết giờ (<= 0) VÀ KHÔNG PHẢI là màn Thiên Thạch (Type 5)
            // (Tức là màn bắn quái thường mà để hết giờ)
            if (timeLeft <= 0.f && currentType != 5)
            {
                // --> XỬ THUA NGAY
                mGame->changeState(std::make_unique<GameOverState>(mGame));
            }
        }
    }
}

void PlayingState::updateHUD()
{
    int currentHp = mPlayer->getHP();
    int maxHp = mPlayer->getMaxHP(); // (Dùng "hàm" (function) "chuẩn" (standard) "của" (of) "chúng ta" (ours))
    mPlayerHPText.setString("HP");
    float hpPercent = (float)currentHp / (float)maxHp;
    if (hpPercent < 0) hpPercent = 0;
    float fullBarWidth = mHpBarBackground.getSize().x;
    mHpBarForeground.setSize(sf::Vector2f(fullBarWidth * hpPercent,
        mHpBarForeground.getSize().y));
    if (hpPercent < 0.3f)
        mHpBarForeground.setFillColor(sf::Color::Red);
    else if (hpPercent < 0.6f)
        mHpBarForeground.setFillColor(sf::Color::Yellow);
    else
        mHpBarForeground.setFillColor(sf::Color::Green);

    // Cập nhật Score (cục bộ)
    mScoreText.setString("SCORE: " + std::to_string(mScore));
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width, 0.f);
    mScoreText.setPosition(800.f - 15.f, 15.f);

    // --- CẬP NHẬT ĐỒNG HỒ ---
    float timeLeft = mWaveManager.getRemainingTime();
    if (timeLeft < 0) timeLeft = 0;

    // Làm tròn số (vd: 14.9s -> 15s)
    int timeInt = static_cast<int>(std::ceil(timeLeft));

    mTimerText.setString("TIME: " + std::to_string(timeInt));
    
    // [KHAI BÁO LẠI timeBounds TẠI ĐÂY VÌ NỘI DUNG CHỮ ĐÃ THAY ĐỔI]
    sf::FloatRect timeBounds = mTimerText.getLocalBounds();
    // Căn chỉnh lại y hệt như lúc khởi tạo
    mTimerText.setOrigin(timeBounds.left + timeBounds.width, 0.f);
    mTimerText.setPosition(800.f - 15.f, 50.f); // Tọa độ phải khớp với ở trên
    
    // Đổi màu đỏ nếu sắp hết giờ (còn dưới 5 giây)
    if (timeInt <= 5) mTimerText.setFillColor(sf::Color::Red);
    else mTimerText.setFillColor(sf::Color::Cyan);

    // Cập nhật HP Boss
    Boss* boss = nullptr;
    for (auto& enemy : mEnemies)
    {
        Boss* potentialBoss = dynamic_cast<Boss*>(enemy.get());
        if (potentialBoss != nullptr)
        {
            boss = potentialBoss;
            break;
        }
    }
    const float BAR_WIDTH = 300.f;
    const float BAR_HEIGHT = 25.f;
    const float PADDING = 20.f;
    float posX = (800.f - BAR_WIDTH) / 2.f;
    float posY = 600.f - PADDING - BAR_HEIGHT;

    if (boss != nullptr)
    {
        float maxHp = (float)boss->getMaxHP();
        float currentHp = (float)boss->getHP();
        if (currentHp < 0) currentHp = 0;
        float hpPercent = currentHp / maxHp;

        mBossHpBarBackground.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
        mBossHpBarBackground.setPosition(posX, posY);
        mBossHpBarForeground.setSize(sf::Vector2f(BAR_WIDTH * hpPercent, BAR_HEIGHT));
        mBossHpBarForeground.setPosition(posX, posY);
    }
    else
    {
        mBossHpBarBackground.setSize(sf::Vector2f(0, 0));
        mBossHpBarForeground.setSize(sf::Vector2f(0, 0));
    }
}

void PlayingState::render(sf::RenderWindow& window)
{
    sf::View fixedView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(fixedView);
    // 1. Vẽ "Nền" (Background)
    window.draw(mBackgroundSprite);

    // 2. Vẽ "Đối Tượng" (Entities)
    for (auto& enemy : mEnemies) {
        enemy->draw(window);
    }
    for (auto& explosion : mExplosions) {
        explosion.draw(window);
    }
    for (auto& powerup : mPowerups) {
        powerup.draw(window);
    }
    mPlayer->draw(window);
    for (auto& bullet : mPlayerBullets) {
        bullet.draw(window);
    }
    for (auto& bullet : mEnemyBullets) {
        bullet.draw(window);
    }

    // 3. Vẽ "HUD" (HUD)
    window.draw(mPlayerHPText);
    window.draw(mHpBarBackground);
    window.draw(mHpBarForeground);
    window.draw(mScoreText);
    window.draw(mTimerText);
    window.draw(mPauseButtonSprite);
    window.draw(mBossHpBarBackground);
    window.draw(mBossHpBarForeground);

    // 4. Vẽ "UI" (UI) "Đè Lên" (Overlay) (Thắng/Thua/Nghỉ)
    if (mWaveManager.getState() == WaveManager::State::Intermission)
    {
        std::string waveStr = "Vòng " + std::to_string(mWaveManager.getNextWaveNumber());
        mWaveNotifyText.setString(waveStr);
        sf::FloatRect waveBounds = mWaveNotifyText.getLocalBounds();
        mWaveNotifyText.setOrigin(waveBounds.left + waveBounds.width / 2.f,
            waveBounds.top + waveBounds.height / 2.f);
        window.draw(mWaveNotifyText);
    }
}

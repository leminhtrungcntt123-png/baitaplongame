#include "WaveManager.h"
#include <cstdlib> // Cho rand()
#include "EnemyLvl2.h"
#include "EnemyLvl1.h"
#include "EnemyLvl3.h"
#include "Boss.h"
#include "Meteor.h" 

// --- HÀM DỰNG (CONSTRUCTOR) 
WaveManager::WaveManager(std::vector<std::unique_ptr<EnemyBase>>& enemies,
    sf::Texture& enemy1Texture, sf::Texture& enemy3Texture,
    sf::Texture& enemy2Texture, sf::Texture& bossTexture,
    sf::Texture& bossBulletTexture,
    sf::Texture& meteoriteTexture) :
    // Kết nối tất cả các tham chiếu
    mEnemiesRef(enemies),
    mEnemy1TextureRef(enemy1Texture),
    mEnemy3TextureRef(enemy3Texture),
    mEnemy2TextureRef(enemy2Texture),
    mBossTextureRef(bossTexture),
    mBossBulletTextureRef(bossBulletTexture),
    mMeteoriteTextureRef(meteoriteTexture),

    // Khởi tạo giá trị ban đầu
    mCurrentWave(-1), // 
    mWaveTimer(3.0f), // Bắt đầu wave đầu tiên sau 3 giây
    mWaveCooldown(3.0f),
    mState(State::Intermission), // Bắt đầu ở trạng thái Nghỉ
    mEnemiesToSpawn(0),
    mEnemiesSpawned(0),
    mEnemySpawnTimer(0.f),
    mBossHasSpawned(false)
{
    setupWaves(); // Ra lệnh: Thiết lập danh sách Vòng Chơi
}

// --- HÀM SETUP ---
void WaveManager::setupWaves()
{
    mWaves.clear(); // Xóa dữ liệu cũ nếu có
    // --- CẤU HÌNH HỆ SỐ ĐỘ KHÓ ---
    float countMult = 1.0f; // Hệ số số lượng (Mặc định Normal)
    float timeMult = 1.0f;  // Hệ số thời gian chờ (Mặc định Normal)

    if (mDifficultyLevel == 0) // EASY
    {
        countMult = 0.7f; // Giảm 30% số lượng quái
        timeMult = 1.2f;  // Tăng 20% thời gian chờ (Quái ra chậm hơn)
    }
    else if (mDifficultyLevel == 2) // HARD
    {
        countMult = 1.5f; // Tăng 50% số lượng quái
        timeMult = 0.7f;  // Giảm 30% thời gian chờ (Quái ra dồn dập)
    }
    // --- THIẾT LẬP CÁC WAVE (Đã áp dụng hệ số) ---
    // Wave 1: Enemy 1
    mWaves.push_back(WaveData{ 1, (int)(10 * countMult), 1.0f * timeMult, 20.f });

    // Wave 2: Enemy 2 
    mWaves.push_back(WaveData{ 2, (int)(15 * countMult), 1.0f * timeMult, 25.f });

    // Wave 3: Enemy 3 
    mWaves.push_back(WaveData{ 3, (int)(15 * countMult), 1.2f * timeMult, 30.f });

    // Wave 4: Boss
    mWaves.push_back(WaveData{ 4, (int)(20 * countMult), 0.0f * timeMult, 60.f });

    // Wave 5: Thiên thạch 
    mWaves.push_back(WaveData{ 5, (int)(200 * countMult), 0.5f * timeMult, 20.f });
}

void WaveManager::update(float deltaTime)
{
    // 1. Nếu đang "Nghỉ" (Intermission) (Chờ "giữa" (between) "các" (the) "wave" (waves))
    if (mState == State::Intermission)
    {
        mWaveTimer += deltaTime;
        if (mWaveTimer >= mWaveCooldown) // (Hết 3 giây nghỉ)
        {
            mCurrentWave++; // Tăng "chỉ số" (index) "wave" (wave)

            // "Kiểm tra" (Check) "xem" (see if) "đã" (has) "hoàn thành" (finished) "TẤT CẢ" (ALL) "các" (the) "wave" (waves) "trong" (in) "vector" (vector) "chưa" (yet)
            if (mCurrentWave >= mWaves.size())
            {
                mState = State::Finished; // Hoàn tất (Sẽ "kích hoạt" (trigger) "màn" (screen) "WonState" (WonState))
                return;
            }

            // "Nếu" (If) "còn" (remaining) "wave" (waves), "BẮT ĐẦU" (START) "wave" (wave) "mới" (new)
            mState = State::Spawning;
            mWaveTimer = 0.f; // Reset "đồng hồ" (timer) (bây giờ "dùng" (used) "để" (to) "đếm" (count) "thời gian" (time) "wave" (wave))
            mEnemiesSpawned = 0;
            mEnemySpawnTimer = 0.f;
            mBossHasSpawned = false;

            // "ĐỌC" (READ) "dữ liệu" (data) "từ" (from) "vector" (vector) `mWaves` (HỆ THỐNG MỚI)
            WaveData& wave = mWaves[mCurrentWave];
            mEnemiesToSpawn = wave.numEnemies;
            mEnemySpawnCooldown = wave.spawnInterval;
        }
    }
    // 2. Nếu đang "Tạo Quái" (Spawning)
    else if (mState == State::Spawning)
    {
        mWaveTimer += deltaTime; // (Đồng hồ "bắt đầu" (starts) "đếm" (counting) "thời gian" (time) "wave" (wave))

        // "Lấy" (Get) "dữ liệu" (data) "wave" (wave) "HIỆN TẠI" (CURRENT)
        WaveData& wave = mWaves[mCurrentWave];

        // "Điều kiện" (Condition) "kết thúc" (end) "cho" (for) "Wave 1-4" (Waves 1-4) (Index 0-3)
        if (mCurrentWave < 4)
        {
            if (isCurrentWaveClear()) // (Phải "giết" (kill) "hết" (all) "quái" (enemies))
            {
                mState = State::Intermission;
                mWaveTimer = 0.f;
                return;
            }
        }
        // "Điều kiện" (Condition) "kết thúc" (end) "cho" (for) "Wave 5" (Wave 5) (Index 4: Thiên Thạch)
        else if (mCurrentWave == 4)
        {
            if (mWaveTimer >= wave.waveTime) // (Sống sót thành công)
            {
                mState = State::Finished;
                return;
            }
        }

        // --- B. Logic "Tạo" (Spawn) "Quái" (Enemies) ---
        if (mEnemiesSpawned < mEnemiesToSpawn)
        {
            mEnemySpawnTimer += deltaTime;
            if (mEnemySpawnTimer >= mEnemySpawnCooldown)
            {
                mEnemySpawnTimer = 0.f;
                spawnEnemy(); // (Gọi "hàm" (function) "tạo quái" (spawn enemy) "riêng" (separate))
            }
        }
    }
}

// --- HÀM "SPAWN ENEMY" (SPAWN ENEMY) "MỚI" (NEW) ---
void WaveManager::spawnEnemy()
{
    // "Lấy" (Get) "loại" (type) "kẻ thù" (enemy) "từ" (from) "vector" (vector) "wave" (wave) "hiện tại" (current)
    int enemyType = mWaves[mCurrentWave].enemyType;

    // (Tọa độ "spawn" (spawn) "chung" (common))
    float spawnX = static_cast<float>(rand() % 750 + 25);
    sf::Vector2f spawnPos(spawnX, -100.f);
    float waypointY = static_cast<float>(rand() % 200 + 50);
    sf::Vector2f waypointPos(spawnX, waypointY);

    // [CÂU TRẢ LỜI SỐ 3]: ĐÂY CHÍNH LÀ CHỖ TẠO ENEMY (Dựa vào enemyType)
    switch (enemyType)
    {
    case 1: // Wave 1 (Enemy 1)
    {
        auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
        enemy->setWaypoint(waypointPos);
        mEnemiesRef.push_back(std::move(enemy));

        mEnemiesSpawned++; // (Đếm số lượng quái đã tạo)
        break;
    }
    case 2: // Wave 2 (Enemy 2) - [ĐÃ SỬA LỖI SỐ 2: ĐỔI TỪ Lvl3 SANG Lvl2]
    {
        auto enemy = std::make_unique<EnemyLvl2>(mEnemy2TextureRef, spawnPos); // Sửa thành Lvl2
        enemy->setWaypoint(waypointPos);
        mEnemiesRef.push_back(std::move(enemy));

        mEnemiesSpawned++;
        break;
    }
    case 3: // Wave 3 (Enemy 3) - [ĐÃ SỬA LỖI SỐ 2: ĐỔI TỪ Lvl2 SANG Lvl3]
    {
        auto enemy = std::make_unique<EnemyLvl3>(mEnemy3TextureRef, spawnPos); // Sửa thành Lvl3
        enemy->setWaypoint(waypointPos);
        mEnemiesRef.push_back(std::move(enemy));

        mEnemiesSpawned++;
        break;
    }
    case 4: // Wave 4 (Boss)
    {
        if (!mBossHasSpawned) // (Chỉ "spawn" (spawn) "1" (one) "lần" (time))
        {
            sf::Vector2f bossSpawnPos(400.f, -200.f);
            sf::Vector2f bossWaypoint(400.f, 150.f);
            auto boss = std::make_unique<Boss>(mBossTextureRef, bossSpawnPos, mBossBulletTextureRef);
            boss->setWaypoint(bossWaypoint);
            mEnemiesRef.push_back(std::move(boss));

            mBossHasSpawned = true;

            mEnemiesSpawned++;
        }
        else
        {
            // Random loại lính (0, 1, hoặc 2)
            int randomMinion = rand() % 3;

            if (randomMinion == 0) // Ra Enemy Lv1
            {
                auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                enemy->setWaypoint(waypointPos);
                mEnemiesRef.push_back(std::move(enemy));
            }
            else if (randomMinion == 1) // Ra Enemy Lv2
            {
                auto enemy = std::make_unique<EnemyLvl2>(mEnemy2TextureRef, spawnPos);
                enemy->setWaypoint(waypointPos);
                mEnemiesRef.push_back(std::move(enemy));
            }
            else // Ra Enemy Lv3
            {
                auto enemy = std::make_unique<EnemyLvl3>(mEnemy3TextureRef, spawnPos);
                enemy->setWaypoint(waypointPos);
                mEnemiesRef.push_back(std::move(enemy));
            }

            mEnemiesSpawned++; // Đếm lính
        }
        break;
    }
    case 5: // Wave 5 (Meteor) - [MƯA THIÊN THẠCH]
    {
        float meteorX = static_cast<float>(rand() % 750 + 25);
        // Meteor rơi từ trên cao (-100.f)
        mEnemiesRef.push_back(std::make_unique<Meteor>(mMeteoriteTextureRef, sf::Vector2f(meteorX, -100.f)));

        mEnemiesSpawned++;
        break;
    }
    }
}
// --- CÁC HÀM "HELPER" (HELPER) "CÒN LẠI" (REMAINING) (Giữ nguyên "như" (as) "cũ" (before)) ---

bool WaveManager::isCurrentWaveClear() const
{
    // Wave 4 5 có thời gian khác nhaunhunwg điều kiện chung là
    // Đã spawn hết quái và danh sách quái rỗng
    return (mEnemiesSpawned >= mEnemiesToSpawn) && mEnemiesRef.empty();
}

bool WaveManager::isAllWavesFinished() const
{
    // (Game "thắng" (is won) = "Trạng thái" (State) "đã" (is) "Finished" (Finished))
    return (mState == State::Finished);
}

WaveManager::State WaveManager::getState() const
{
    return mState;
}

int WaveManager::getNextWaveNumber() const
{
    // (Vì "mCurrentWave" (mCurrentWave) "bắt đầu" (starts) "từ" (at) "-1" (negative one), "Wave 0" (Wave 0) "sẽ" (will) "hiển thị" (show) "là" (as) "Vòng 1" (Wave 1))
    return mCurrentWave + 2; // (Sửa "lỗi" (bug) "logic" (logic) "nhỏ" (off-by-one))
}

int WaveManager::getCurrentEnemyType() const
{
    if (mCurrentWave >= 0 && mCurrentWave < mWaves.size())
    {
        return mWaves[mCurrentWave].enemyType;
    }
    return 0; // Mặc định
}

float WaveManager::getRemainingTime() const
{
    if (mCurrentWave >= 0 && mCurrentWave < mWaves.size())
    {
        // [LOGIC MỚI]: Nếu đang Nghỉ hoặc Chờ -> Giữ nguyên thời gian gốc (không trừ)
        // Để đồng hồ không bị nhảy số lung tung khi chuyển màn
        if (mState == State::Intermission || mState == State::Waiting)
        {
            return mWaves[mCurrentWave].waveTime;
        }

        // Nếu đang chơi thật -> Trả về (Tổng giờ - Giờ đã trôi qua)
        return mWaves[mCurrentWave].waveTime - mWaveTimer;
    }
    return 0.f;
}

void WaveManager::setDifficulty(int level)
{
    // Lưu độ khó mới
    mDifficultyLevel = level;
    setupWaves();
}

#include "WaveManager.h"
#include <cstdlib> // Cho rand()
#include "EnemyLvl2.h"
#include "EnemyLvl1.h"
#include "EnemyLvl3.h"
#include "Boss.h"

// 1. Hàm dựng: Khởi tạo các giá trị
WaveManager::WaveManager(std::vector<std::unique_ptr<EnemyBase>>& enemies,
    sf::Texture& enemy1Tex,
    sf::Texture& enemy3Tex,
    sf::Texture& enemy2Tex, // <-- Thêm
    sf::Texture& bossTex,
    sf::Texture& bossBulletTex)
    : mEnemiesRef(enemies),           // Gán tham chiếu vector
    mEnemy1TextureRef(enemy1Tex), // Gán tham chiếu texture 1
    mEnemy3TextureRef(enemy3Tex),  // Gán tham chiếu texture 3
    mEnemy2TextureRef(enemy2Tex),
    mBossTextureRef(bossTex),
    mBossBulletTextureRef(bossBulletTex)
{
    // --- "CẮT" CÁC GIÁ TRỊ TỪ GAME::GAME() DÁN VÀO ĐÂY ---
    mEnemySpawnTimer = 0.f;
    mEnemySpawnCooldown = 1.0f; // Tần suất tạo quái (1 giây/con)
    // --- KHỞI TẠO BIẾN MỚI ---
    mCurrentWave = 0; // Sẽ +1 ngay khi bắt đầu
    mWaveCooldown = 3.0f; // Chờ 3 giây giữa các wave
    mWaveTimer = 3.0f;    // Bắt đầu wave đầu tiên ngay lập tức
    mState = State::Waiting; // Bắt đầu ở trạng thái "Chờ"

    // Khởi tạo các giá trị "rỗng" ban đầu
    mEnemiesToSpawn = 0;
    mEnemiesSpawned = 0;
    mBossHasSpawned = false; // Ban đầu chưa spawn
}

// 2. Hàm Update:
void WaveManager::update(float deltaTime)
{
    // 1. Nếu đang "CHỜ" (Waiting), kiểm tra xem có bắt đầu wave mới không
    if (mState == State::Waiting)
    {
        mWaveTimer += deltaTime;
        if (mWaveTimer >= mWaveCooldown) // (mWaveCooldown = 3 giây)
        {
            // --- ĐÃ ĐẾN LÚC BẮT ĐẦU WAVE MỚI ---
            mCurrentWave++;
            mState = State::Spawning; // Chuyển sang trạng thái "Đang tạo"

            mEnemiesSpawned = 0; // Reset số quái đã tạo
            mWaveTimer = 0.f;    // Reset đồng hồ (giờ dùng để spawn)

            // --- ĐỊNH NGHĨA CÁC WAVE (Code cũ của bạn) ---
            if (mCurrentWave == 1) {
                mEnemiesToSpawn = 10;
                mEnemySpawnCooldown = 1.5f;
            }
            else if (mCurrentWave == 2) {
                mEnemiesToSpawn = 15;
                mEnemySpawnCooldown = 1.0f;
            }
            else if (mCurrentWave == 3) {
                mEnemiesToSpawn = 20;
                mEnemySpawnCooldown = 0.7f;
            }
            else if (mCurrentWave == 4)
            {
                // Wave 4: 20 quái nhỏ
                mEnemiesToSpawn = 20;
                mEnemySpawnCooldown = 1.0f; // Tần suất 1 giây/con
                mBossHasSpawned = false; // Reset cờ
            }
            else
            {
                // Đã xong 4 wave
                mState = State::Finished;
                mEnemiesToSpawn = 0;
            }
        }
    }
    // 2. Nếu đang "TẠO QUÁI" (Spawning), làm 2 việc:
    else if (mState == State::Spawning)
    {
        // 1. KIỂM TRA "DỌN SẠCH" WAVE (Code cũ)
        if (mEnemiesSpawned >= mEnemiesToSpawn && mEnemiesRef.empty())
        {
            if (mCurrentWave >= 4) // (Đã sửa thành 4)
                mState = State::Finished;
            else
                mState = State::Intermission;

            mWaveTimer = 0.f;
            return;
        }

        // --- LOGIC MỚI: SPAWN BOSS TRƯỚC (CHO WAVE 4) ---
        // (Chạy 1 lần duy nhất)
        if (mCurrentWave == 4 && !mBossHasSpawned)
        {
            // Tọa độ ra đời của Boss
            sf::Vector2f spawnPos(400.f, -200.f);
            sf::Vector2f waypointPos(400.f, 150.f);

            // "THI CÔNG" BOSS (Truyền texture đạn MỚI)
            auto boss = std::make_unique<Boss>(mBossTextureRef, spawnPos, mBossBulletTextureRef);
            boss->setWaypoint(waypointPos);
            mEnemiesRef.push_back(std::move(boss));

            mBossHasSpawned = true; // Đánh dấu đã spawn Boss
        }

        // 2. LOGIC SPAWN QUÁI NHỎ (Luôn luôn chạy)
        if (mEnemiesSpawned < mEnemiesToSpawn)
        {
            mEnemySpawnTimer += deltaTime;
            if (mEnemySpawnTimer >= mEnemySpawnCooldown)
            {
                mEnemySpawnTimer = 0.f;

                // (Tọa độ spawn)
                float spawnX = static_cast<float>(rand() % 750);
                sf::Vector2f spawnPos(spawnX, -100.f);
                float waypointY = static_cast<float>(rand() % 200 + 50);
                sf::Vector2f waypointPos(spawnX, waypointY);

                // --- LOGIC TẠO QUÁI NHỎ (Cũ + Mới) ---

                // Wave 1: Chỉ Lvl1
                if (mCurrentWave == 1) {
                    auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                    enemy->setWaypoint(waypointPos);
                    mEnemiesRef.push_back(std::move(enemy));
                }
                // Wave 2: Lvl1 + Lvl2
                else if (mCurrentWave == 2) {
                    // (code cũ wave 2)
                    if (rand() % 10 < 7) {
                        auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                    else {
                        auto enemy = std::make_unique<EnemyLvl2>(mEnemy2TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                }
                // Wave 3 VÀ 4: Lvl1 + Lvl2 + Lvl3
                else if (mCurrentWave == 3 || mCurrentWave == 4)
                {
                    // (code cũ wave 3)
                    int roll = rand() % 10;
                    if (roll < 5) {
                        auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                    else if (roll < 8) {
                        auto enemy = std::make_unique<EnemyLvl2>(mEnemy2TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                    else {
                        auto enemy = std::make_unique<EnemyLvl3>(mEnemy3TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                }

                mEnemiesSpawned++; // Chỉ tăng số quái nhỏ
            }
        }
    } // (Kết thúc "else if (mState == State::Spawning)")
    // 3. Nếu đang "KHOẢNG NGHỈ" (Intermission), chỉ đếm giờ
    else if (mState == State::Intermission)
    {
        mWaveTimer += deltaTime;
        if (mWaveTimer >= mWaveCooldown) // (Hết 3 giây nghỉ)
        {
            // Quay lại "Chờ" (vòng lặp update tiếp theo sẽ kích hoạt wave mới)
            mState = State::Waiting;
            mWaveTimer = 0.f; // Reset đồng hồ chờ
        }
    }
}
// 3. Hàm kiểm tra "dọn" wave
bool WaveManager::isCurrentWaveClear() const
{
    // Wave clear = Đã spawn hết (Total) VÀ
    //               Vector quái (mEnemiesRef) rỗng (empty)
    return (mEnemiesSpawned >= mEnemiesToSpawn) && mEnemiesRef.empty();
}

// Thêm hàm mới này
bool WaveManager::isAllWavesFinished() const
{
    // Game thắng = Trạng thái "Finished" VÀ wave cuối đã được dọn sạch
    return (mState == State::Finished) && isCurrentWaveClear();
}

// --- THÊM 2 HÀM NÀY VÀO CUỐI FILE ---

WaveManager::State WaveManager::getState() const
{
    return mState;
}

int WaveManager::getNextWaveNumber() const
{
    // Nếu đang "Intermission" hoặc "Waiting" sau wave 1, nó sẽ báo "Vòng 2"
    return mCurrentWave + 1;
}
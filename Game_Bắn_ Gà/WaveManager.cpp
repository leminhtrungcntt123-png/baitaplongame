#include "WaveManager.h"
#include <cstdlib> // Cho rand()

// 1. Hàm dựng: Khởi tạo các giá trị
WaveManager::WaveManager(std::vector<std::unique_ptr<EnemyBase>>& enemies,
    sf::Texture& enemy1Tex,
    sf::Texture& enemy3Tex)
    : mEnemiesRef(enemies),           // Gán tham chiếu vector
    mEnemy1TextureRef(enemy1Tex), // Gán tham chiếu texture 1
    mEnemy3TextureRef(enemy3Tex)  // Gán tham chiếu texture 3
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
            else {
                mState = State::Finished; // Đã xong 3 wave
                mEnemiesToSpawn = 0;
            }
        }
    }
    // 2. Nếu đang "TẠO QUÁI" (Spawning), làm 2 việc:
    else if (mState == State::Spawning)
    {
        // 2A. KIỂM TRA XEM ĐÃ "DỌN SẠCH" WAVE CHƯA?
        // (Đã spawn đủ VÀ vector quái rỗng)
        if (mEnemiesSpawned >= mEnemiesToSpawn && mEnemiesRef.empty())
        {
            // --- WAVE ĐÃ ĐƯỢC DỌN SẠCH! ---

            if (mCurrentWave >= 3) // Nếu đây là wave cuối (Wave 3)
            {
                mState = State::Finished; // Xong game
            }
            else
            {
                mState = State::Intermission; // Chuyển sang "Khoảng nghỉ"
                mWaveTimer = 0.f; // Reset đồng hồ (để đếm 3 giây nghỉ)
            }
            return; // Dừng, không spawn nữa
        }

        // 2B. NẾU CHƯA DỌN SẠCH, TIẾP TỤC SPAWN QUÁI (Logic cũ)
        if (mEnemiesSpawned < mEnemiesToSpawn)
        {
            mEnemySpawnTimer += deltaTime;
            if (mEnemySpawnTimer >= mEnemySpawnCooldown)
            {
                mEnemySpawnTimer = 0.f;
                // ... (Toàn bộ code rand() và push_back() quái cũ của bạn) ...
                // (Copy/Paste toàn bộ code spawn cũ vào đây)
                float spawnX = static_cast<float>(rand() % 750);
                sf::Vector2f spawnPos(spawnX, -100.f);
                float waypointY = static_cast<float>(rand() % 200 + 50);
                sf::Vector2f waypointPos(spawnX, waypointY);
                if (mCurrentWave == 1) {
                    auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                    enemy->setWaypoint(waypointPos);
                    mEnemiesRef.push_back(std::move(enemy));
                }
                else {
                    if (rand() % 10 < 7) {
                        auto enemy = std::make_unique<EnemyLvl1>(mEnemy1TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                    else {
                        auto enemy = std::make_unique<EnemyLvl3>(mEnemy3TextureRef, spawnPos);
                        enemy->setWaypoint(waypointPos);
                        mEnemiesRef.push_back(std::move(enemy));
                    }
                }
                mEnemiesSpawned++;
                // ... (Kết thúc code spawn cũ) ...
            }
        }
    }
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
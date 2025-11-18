#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "EnemyBase.h"
#include "EnemyLvl1.h"
#include "EnemyLvl3.h"

struct WaveData
{
    int enemyType;      // (Loại "1" (1) "là" (is) "Enemy1", "4" (4) "là" (is) "Boss", "5" (5) "là" (is) "Meteor" (Meteor)...)
    int numEnemies;     // (Tổng "số" (number) "quái" (enemies) "sẽ" (will be) "tạo" (spawned))
    float spawnInterval;// (Thời gian "chờ" (wait) "giữa" (between) "mỗi" (each) "lần" (time) "tạo" (spawn))
    float waveTime;     // (Thời gian "tối đa" (maximum) "của" (of) "vòng" (wave) "đó" (that))
};

class WaveManager
{
public:
    enum class State 
    { 
        Spawning, 
        Waiting, 
        Finished, 
        Intermission
    };
    // Hàm dựng: Nhận "tham chiếu" (&) đến các tài nguyên của Game
    // Nó cần "biết" vector Enemies và các Texture
    WaveManager(std::vector<std::unique_ptr<EnemyBase>>& enemies,
        sf::Texture& enemy1Tex,
        sf::Texture& enemy3Tex,
        sf::Texture& enemy2Tex,
        sf::Texture& bossTex,
        sf::Texture& bossBulletTex,
        sf::Texture& meteoriteTexture);

    void setupWaves();
    // Hàm update (chứa toàn bộ logic spawn cũ)
    void update(float deltaTime);

    // Hàm kiểm tra (để Game biết đã thắng wave chưa)
    bool isCurrentWaveClear() const;
    bool isAllWavesFinished() const;
    State getState() const;       // Lấy trạng thái hiện tại (Đang Spawning? Intermission?)
    int getNextWaveNumber() const; // Lấy số của wave *tiếp theo*
    float getRemainingTime() const; // Lấy thời gian còn lại
    int getCurrentEnemyType() const; // Để biết đang là wave thường hay wave thiên thạch
    void spawnEnemy();
    int mDifficultyLevel; // 0 = Easy, 1 = Normal, 2 = Hard
    void setDifficulty(int level);
private:
    // --- CÁC BIẾN MÀ CHÚNG TA "CẮT" TỪ GAME.H ---
    float mEnemySpawnTimer;
    float mEnemySpawnCooldown;
    int mEnemiesToSpawn;
    int mEnemiesSpawned;
    bool mBossHasSpawned; // Đã spawn Boss ở Wave 4 chưa?

    // --- BIẾN MỚI ---
    State mState;         // Trạng thái của Manager (Đang tạo? Đang chờ?)
    int mCurrentWave;     // Đang ở wave mấy?
    float mWaveCooldown;  // Thời gian chờ giữa các wave
    float mWaveTimer;     // Đồng hồ đếm chờ

    std::vector<WaveData> mWaves;

    // --- CÁC THAM CHIẾU ĐẾN "TÀI NGUYÊN" GỐC CỦA GAME ---
    std::vector<std::unique_ptr<EnemyBase>>& mEnemiesRef; // Tham chiếu tới mEnemies
    sf::Texture& mEnemy1TextureRef; // Tham chiếu tới mEnemy1Texture
    sf::Texture& mEnemy3TextureRef; // Tham chiếu tới mEnemy3Texture
    sf::Texture& mEnemy2TextureRef;
    sf::Texture& mBossTextureRef;
    sf::Texture& mBossBulletTextureRef;
    sf::Texture& mMeteoriteTextureRef;
};

#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "EnemyBase.h"
#include "EnemyLvl1.h"
#include "EnemyLvl3.h"

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
        sf::Texture& bossBulletTex);

    // Hàm update (chứa toàn bộ logic spawn cũ)
    void update(float deltaTime);

    // Hàm kiểm tra (để Game biết đã thắng wave chưa)
    bool isCurrentWaveClear() const;
    bool isAllWavesFinished() const;
    State getState() const;       // Lấy trạng thái hiện tại (Đang Spawning? Intermission?)
    int getNextWaveNumber() const; // Lấy số của wave *tiếp theo*
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

    // --- CÁC THAM CHIẾU ĐẾN "TÀI NGUYÊN" GỐC CỦA GAME ---
    std::vector<std::unique_ptr<EnemyBase>>& mEnemiesRef; // Tham chiếu tới mEnemies
    sf::Texture& mEnemy1TextureRef; // Tham chiếu tới mEnemy1Texture
    sf::Texture& mEnemy3TextureRef; // Tham chiếu tới mEnemy3Texture
    sf::Texture& mEnemy2TextureRef;
    sf::Texture& mBossTextureRef;
    sf::Texture& mBossBulletTextureRef;
};

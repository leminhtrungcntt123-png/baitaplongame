#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bullet.h"
#include "EnemyBase.h" // (Các include phải đúng)
#include "Powerup.h"
#include "Player.h"
#include <SFML/Audio.hpp>
#include "Explosion.h"
#include "WaveManager.h" // (Phải có WaveManager.h)


enum class GameState
{
    MainMenu, // (Thêm ở Bước 7 Mới)
    Playing,
    GameOver,
    Won
};

class Game
{
public:
    Game();
    void run();

private:
    // --- Hàm "Phòng Ban" (Refactor) ---
    void processInput();
    void update(float deltaTime);
    void render();
    void loadAssets();
    void updatePlayer(float deltaTime);
    void updateBullets(float deltaTime);
    void updateCollisions();
    void updateEntities(float deltaTime);
    void checkGameState();
    void updateHUD(); // Cập nhật HP và Score

    // --- Biến SFML ---
    sf::RenderWindow mWindow;
    sf::Clock mDeltaClock;

    // --- Textures ---
    sf::Texture mPlayerTexture;
    sf::Texture mBulletTexture;
    sf::Texture mEnemy1Texture;
    sf::Texture mEnemy3Texture;
    sf::Texture mEnemy2Texture; // (Thêm ở Bước 9)
    sf::Texture mBossTexture;   // (Thêm ở Bước 9)
    sf::Texture mPowerupTexture;
    sf::Texture mBackgroundTexture;
    sf::Texture mEnemyBulletTexture;
    sf::Texture mBossBulletTexture; // Cho BulletBoss.png

    // --- Âm thanh ---
    sf::SoundBuffer mShootBuffer;
    sf::SoundBuffer mExplosionBuffer;
    sf::SoundBuffer mPowerupBuffer;
    sf::Sound mShootSound;
    sf::Sound mExplosionSound;
    sf::Sound mPowerupSound;

    // --- Đối tượng Game ---
    std::unique_ptr<Player> mPlayer;
    sf::Sprite mBackgroundSprite;
    std::vector<Bullet> mPlayerBullets;
    std::vector<Bullet> mEnemyBullets;
    std::vector<std::unique_ptr<EnemyBase>> mEnemies;
    std::vector<Powerup> mPowerups;
    std::vector<Explosion> mExplosions;
    WaveManager mWaveManager; // (Thêm ở Bước 4B)

    // --- Trạng thái Game ---
    float mBulletSpeed;
    GameState mGameState;

    // --- Giao Diện (UI) ---
    sf::Font mFont;
    sf::Text mWinText;
    sf::Text mGameOverText;
    sf::Text mStartButton; // (Thêm ở Bước 7 Mới)
    sf::Text mExitButton;  // (Thêm ở Bước 7 Mới)

    // (Thêm ở Bước 8)
    sf::Text mWaveNotifyText; 
    sf::Text mPlayerHPText;   
    int mScore;           // Biến lưu điểm
    sf::Text mScoreText;  // Biến hiển thị điểm

}; 
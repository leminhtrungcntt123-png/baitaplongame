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
    Paused,
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
    void updateView();

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
    // Chỉnh giao diện ở menu
    sf::Texture mBossBulletTexture; // Cho BulletBoss.png
    sf::Texture mMenuBackgroundTexture;  // giao diện background
    sf::Sprite  mMenuBackgroundSprite;
    sf::Texture mTitleTexture;              // title của game
    sf::Sprite  mTitleSprite;
    sf::Texture mPlayButtonTexture;         // nút play của game
    sf::Sprite  mPlayButtonSprite;

    // --- Âm thanh ---
    sf::SoundBuffer mShootBuffer;
    sf::SoundBuffer mExplosionBuffer;
    sf::SoundBuffer mPowerupBuffer;
    sf::Sound mShootSound;
    sf::Sound mExplosionSound;
    sf::Sound mPowerupSound;

    // --- Đối tượng Game ---
    std::unique_ptr<Player> mPlayer;
    sf::Sprite mBackgroundSprite;   // background cho play_game
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
    // Tạo nút pause trong game play
    sf::Texture mPauseButtonTexture; // Texture cho nút "||"
    sf::Sprite  mPauseButtonSprite;  // Sprite cho nút "||"

    sf::RectangleShape mPauseOverlay; // Lớp "phủ mờ" (dim overlay)

    sf::Text mPauseResumeButton; // Chữ "CHƠI TIẾP"
    sf::Text mPauseExitButton;   // Chữ "THOÁT"

    // (Thêm ở Bước 8)
    sf::Text mWaveNotifyText; 
    sf::Text mPlayerHPText;   
    // --- THÊM "TÚI" MỚI CHO HP BAR ---
    sf::RectangleShape mHpBarBackground; // "Nền" (ví dụ: màu xám)
    sf::RectangleShape mHpBarForeground;

    // Điểm 
    int mScore;           // Biến lưu điểm
    sf::Text mScoreText;  // Biến hiển thị điểm
}; 

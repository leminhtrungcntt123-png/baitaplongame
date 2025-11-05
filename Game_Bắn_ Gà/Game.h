#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bullet.h"
#include "EnemyBase.h"
#include "Powerup.h"
#include "Player.h"
#include <SFML/Audio.hpp>
#include "Explosion.h"
#include "WaveManager.h"
// Định nghĩa các trạng thái của "cỗ máy game"
enum class GameState
{
    MainMenu, // <-- THÊM DÒNG NÀY
    Playing,
    GameOver,
    Won
};
class Game
{
public:
    // Hàm dựng: Sẽ khởi tạo cửa sổ và tải assets
    Game();

    // Hàm chính: Chứa vòng lặp while(isOpen)
    void run();

private:
    // --- CÁC HÀM "PHÒNG BAN" CHUYÊN BIỆT ---
    void processInput();        // Xử lý tất cả input (Bắn, Di chuyển, Tắt)
    void update(float deltaTime); // Chạy tất cả logic game
    void render();                // Vẽ mọi thứ lên màn hình

    // --- HÀM HỖ TRỢ KHỞI TẠO ---
    void loadAssets(); // Tập trung code tải assets vào đây

    // --- CÁC HÀM "PHÒNG BAN" MỚI (PRIVATE) ---
    void updatePlayer(float deltaTime);
    void updateBullets(float deltaTime);
    void updateCollisions();
    void updateEntities(float deltaTime); // (Gộp Quái, Nổ, Vật phẩm)
    void checkGameState();

    sf::RenderWindow mWindow;
    sf::Clock mDeltaClock;

    // Textures
    sf::Texture mPlayerTexture;
    sf::Texture mBulletTexture;
    sf::Texture mEnemy1Texture;
    sf::Texture mEnemy3Texture;
    sf::Texture mPowerupTexture;
    sf::Texture mBackgroundTexture;
    sf::Texture mEnemyBulletTexture;
    // --- THÊM BIẾN ÂM THANH VÀO ĐÂY ---
    // "Đĩa nhạc" (Buffers - Lưu trữ dữ liệu âm thanh)
    sf::SoundBuffer mShootBuffer;
    sf::SoundBuffer mExplosionBuffer;
    sf::SoundBuffer mPowerupBuffer;

    // "Loa" (Sounds - Dùng để phát)
    sf::Sound mShootSound;
    sf::Sound mExplosionSound;
    sf::Sound mPowerupSound;

    // Đối tượng game
    std::unique_ptr<Player> mPlayer;
    sf::Sprite mBackgroundSprite;
    std::vector<Bullet> mPlayerBullets; // <-- Đạn của Tàu (Đổi tên từ mBullets)
    std::vector<Bullet> mEnemyBullets;  // <-- Đạn của Quái (Thêm mới)
    std::vector<std::unique_ptr<EnemyBase>> mEnemies;
    std::vector<Powerup> mPowerups;
    std::vector<Explosion> mExplosions;
    WaveManager mWaveManager;

    // Trạng thái game
    float mBulletSpeed;
    GameState mGameState;          // Trạng thái đã thắng hay chưa
    // --- BIẾN GIAO DIỆN (UI) (THÊM VÀO ĐÂY) ---
    sf::Font mFont;
    sf::Text mWinText;
    sf::Text mGameOverText;
    sf::Text mStartButton; // Nút "Bắt đầu"
    sf::Text mExitButton;  // Nút "Thoát"
    // --- THÊM 2 DÒNG NÀY VÀO ---
    sf::Text mWaveNotifyText; // Chữ "Vòng 2"
    sf::Text mPlayerHPText;   // Chữ "HP: 10"
};

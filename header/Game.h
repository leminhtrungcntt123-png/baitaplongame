#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <stack>
#include "Bullet.h"
#include "Explosion.h"
#include "BaseState.h"

enum class Difficulty
{
    Easy,   // Dễ
    Normal, // Thường
    Hard    // Khó
};

class Game
{
public:
    Game();
    void run();

    // --- "HÀM QUẢN LÝ" (MANAGER FUNCTIONS) "MỚI" (NEW) ---
    void pushState(std::unique_ptr<BaseState> state);
    void popState();
    void changeState(std::unique_ptr<BaseState> state);
    BaseState *getCurrentState(); // (Lấy "Chuyên gia" (Specialist) "hiện tại" (current))

    // --- "TÚI ĐỒ" (PUBLIC ASSETS) (CHO "CHUYÊN GIA" (SPECIALISTS) "SỬ DỤNG" (TO USE)) ---
    // (Tất cả "Tài Sản" (Assets) "bây giờ" (now) "là" (are) "public" (public) "để" (for) "các" (the) "State" (States) "lấy" (to get))

    sf::RenderWindow mWindow;

    // --- Textures ---
    sf::Texture mPlayerTexture;
    sf::Texture mBulletTexture;
    sf::Texture mEnemy1Texture;
    sf::Texture mEnemy3Texture;
    sf::Texture mEnemy2Texture;
    sf::Texture mBossTexture;
    sf::Texture mPowerupTexture;
    sf::Texture mBackgroundTexture;
    sf::Texture mSelectModeTexture;
    sf::Texture mEnemyBulletTexture;
    sf::Texture mBossBulletTexture;
    sf::Texture mMenuBackgroundTexture;
    sf::Texture mTitleTexture;
    sf::Texture mPlayButtonTexture;
    sf::Texture mPauseButtonTexture;
    
    sf::Texture mPlayerVIPTexture;
    sf::Texture mBulletPlayerVIPTexture;
    sf::Texture mItemChangePlayerTexture;
    sf::Texture mMeteoriteTexture;
    sf::Texture mItemBuffHPTexture;
    sf::Texture mLoseBackground;
    sf::Texture mWonBackground;
    // --- 2. BIẾN LƯU LỰA CHỌN ---
    Difficulty mDifficulty;

    // --- Âm thanh ---
    sf::SoundBuffer mShootBuffer;
    sf::SoundBuffer mExplosionBuffer;
    sf::SoundBuffer mPowerupBuffer;
    sf::Sound mShootSound;
    sf::Sound mExplosionSound;
    sf::Sound mPowerupSound;

    // --- Giao Diện (UI) ---
    sf::Font mFont;
    bool mFontLoaded = true;

private:
    // --- "HÀM" (FUNCTIONS) "PHÒNG BAN" (DEPARTMENT) "GỌN GÀNG" (CLEAN) ---
    void processInput();
    void update(float deltaTime);
    void render();
    void loadAssets();
    void updateView(); // (Giữ lại "hàm" (function) "sửa lỗi" (bug fix) "Letterbox" (Letterbox))

    // --- "BIẾN" (VARIABLES) "HỆ THỐNG" (SYSTEM) ---
    sf::Clock mDeltaClock;

    std::stack<std::unique_ptr<BaseState>> mStates;
};

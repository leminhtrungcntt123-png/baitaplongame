#pragma once
#include "BaseState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Game.h" // (Include "Game" (Game) "để" (to) "lấy" (get) "Tài Sản" (Assets) "và" (and) "gọi" (call) "pushState")
#include "Player.h" // (CHUYÊN GIA "NÀY" (THIS) "SỞ HỮU" (OWNS) "PLAYER")
#include "EnemyBase.h"
#include "WaveManager.h"
#include "Bullet.h"
#include "Powerup.h"
#include "Explosion.h"
#include "CollisionManager.h"
#include "Boss.h" // (Cần "include" (include) "Boss.h" "để" (to) "biết" (know) "cách" (how) "ép kiểu" (dynamic_cast))


// "Chuyên gia" (Specialist) "này" (this) "ký" (signs) "vào" (the) "Bản Hợp Đồng" (Contract) "BaseState" (BaseState)
class PlayingState : public BaseState
{
public:
    PlayingState(Game* game); // "Hàm Dựng" (Constructor)

    // --- "THI CÔNG" (IMPLEMENT) "3" (THREE) "ĐIỀU KHOẢN" (CLAUSES) "HỢP ĐỒNG" (CONTRACT) ---
    virtual void processInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    // --- "HÀM" (FUNCTIONS) "NỘI BỘ" (INTERNAL) (Code "CẮT" (CUT) "từ" (from) "Game.cpp") ---
    // (Đây "là" (are) "các" (the) "hàm" (functions) "Phòng Ban" (Department) "cũ" (old))
    void updatePlayer(float deltaTime);
    void updateBullets(float deltaTime);
    void updateCollisions();
    void updateEntities(float deltaTime);
    void checkGameState();
    void updateHUD();

    // --- "NHÀ KHO RIÊNG" (PRIVATE STORAGE) ---
    // (Tất cả "biến" (variables) "logic" (logic) "gameplay" (gameplay) "đã" (have) "di chuyển" (moved) "đến" (to) "đây" (here) "từ" (from) "Game.h")

    // (Con trỏ "tới" (to) "Tài Sản" (Assets) "chung" (global))
    sf::Texture& mBackgroundTexture;
    sf::Texture& mPauseButtonTexture;
    sf::Texture& mEnemyBulletTexture;
    sf::Texture& mPowerupTexture;
    sf::Font& mFont;

    // (Đối tượng "Game" (Game) "cụ thể" (specific))
    sf::Sprite mBackgroundSprite;
    std::unique_ptr<Player> mPlayer;
    std::vector<Bullet> mPlayerBullets;
    std::vector<Bullet> mEnemyBullets;
    std::vector<std::unique_ptr<EnemyBase>> mEnemies;
    std::vector<Powerup> mPowerups;
    std::vector<Explosion> mExplosions;
    WaveManager mWaveManager;
    float mBulletSpeed;

    // (Trạng thái "Game" (Game) "cục bộ" (local))
    // ("Game.h" "không" (no longer) "biết" (knows) "về" (about) "GameOver" (GameOver) "hay" (or) "Won" (Won). 
    //  "Chỉ" (Only) "PlayingState" (PlayingState) "biết" (knows) "về" (about) "chúng" (them).)
    enum class PlayState { Playing, Won, GameOver };
    PlayState mPlayState;

    // (Các "biến" (variables) "UI" (UI) "của" (of) "Gameplay" (Gameplay))
    sf::Text mWinText;
    sf::Text mGameOverText;
    sf::Text mWaveNotifyText;
    sf::Text mPlayerHPText;
    sf::RectangleShape mHpBarBackground;
    sf::RectangleShape mHpBarForeground;
    sf::Text mScoreText;
    sf::Text mTimerText;
    int mScore;
    int mKillCount; // Đếm số quái đã giết để tính rơi đồ
    // (Các "biến" (variables) "UI" (UI) "của" (of) "Boss" (Boss))
    sf::RectangleShape mBossHpBarBackground;
    sf::RectangleShape mBossHpBarForeground;

    // (Nút Pause)
    sf::Sprite  mPauseButtonSprite;
};

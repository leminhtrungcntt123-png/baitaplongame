#pragma once
#include <vector>
#include <memory>
#include "Bullet.h"
#include "EnemyBase.h"
#include "Powerup.h"
#include "Player.h"

// Chúng ta dùng namespace cho các "hệ thống" (systems)
// không cần lưu trữ trạng thái (stateless)
namespace CollisionManager
{
    // Một hàm tĩnh duy nhất, nhận 2 danh sách
    // Chúng ta dùng "&" (tham chiếu) để có thể XÓA đạn và kẻ thù
    // ra khỏi danh sách GỐC trong main.cpp
    void ProcessCollisions(std::vector<Bullet>& bullets,
        std::vector<std::unique_ptr<EnemyBase>>& enemies);
    bool ProcessPlayerCollisions(Player& player,
        std::vector<Powerup>& powerups);
    // Kiểm tra va chạm giữa Tàu và Kẻ thù
    void ProcessPlayerHits(Player& player,
        std::vector<std::unique_ptr<EnemyBase>>& enemies,
        std::vector<Bullet>& enemyBullets);
}
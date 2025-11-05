#include "CollisionManager.h"
#include <SFML/Graphics.hpp> // Cần cho getGlobalBounds() và intersects()
#include "Powerup.h"
#include "Player.h"

// Định nghĩa hàm nằm trong namespace CollisionManager
void CollisionManager::ProcessCollisions(std::vector<Bullet>& bullets,
    std::vector<std::unique_ptr<EnemyBase>>& enemies)
{
    // --- DI CHUYỂN TOÀN BỘ LOGIC TỪ MAIN.CPP SANG ĐÂY ---

    // Dùng vòng lặp "for" ngược để an toàn khi xóa
    for (int i = enemies.size() - 1; i >= 0; i--)
    {
        for (int j = bullets.size() - 1; j >= 0; j--)
        {
            // Lấy "hộp va chạm" (hitbox) của kẻ thù
            sf::FloatRect enemyBounds = enemies[i]->sprite.getGlobalBounds();

            // Lấy "hộp va chạm" (hitbox) của đạn
            sf::FloatRect bulletBounds = bullets[j].sprite.getGlobalBounds();

            // Kiểm tra va chạm
            if (enemyBounds.intersects(bulletBounds))
            {
                // --- ĐÃ CÓ VA CHẠM! ---

                // 1. Kẻ thù nhận sát thương
                enemies[i]->takeDamage(bullets[j].getDamage());

                // 2. Xóa viên đạn
                bullets.erase(bullets.begin() + j);

                // 3. Thoát khỏi vòng lặp đạn (vì đạn đã biến mất)
                break;
            }
        }
    }
}

// Code cho hàm va chạm mới: Player vs Powerup
bool CollisionManager::ProcessPlayerCollisions(Player& player,
    std::vector<Powerup>& powerups)
{
    // Lấy hitbox của tàu từ Player
    sf::FloatRect playerBounds = player.getBounds();

    for (int i = powerups.size() - 1; i >= 0; i--)
    {
        sf::FloatRect powerupBounds = powerups[i].sprite.getGlobalBounds();

        if (playerBounds.intersects(powerupBounds))
        {
            // --- ĐÃ NHẶT ĐƯỢC VẬT PHẨM ---
            if (powerups[i].type == Powerup::UpgradeGun)
            {
                // Gọi hàm của Player để tự nâng cấp
                player.upgradeGun();
            }
            powerups.erase(powerups.begin() + i);
            return true; // <-- BÁO LÀ ĐÃ NHẶT
        }
    }
    return false; // Không nhặt
}
// Logic đạn của quai va chạm với tàu
void CollisionManager::ProcessPlayerHits(Player& player,
    std::vector<std::unique_ptr<EnemyBase>>& enemies,
    std::vector<Bullet>& enemyBullets) // <-- Chữ ký MỚI
{
    // Nếu tàu không còn sống, không cần kiểm tra
    if (!player.isAlive()) return;

    sf::FloatRect playerBounds = player.getBounds();

    // --- 1.Va chạm Tàu vs Quái (Đâm nhau) ---
    for (auto& enemy : enemies)
    {
        sf::FloatRect enemyBounds = enemy->sprite.getGlobalBounds();

        // Kiểm tra va chạm
        if (playerBounds.intersects(enemyBounds))
        {
            // Va chạm Tàu vs Quái = Chết ngay
            player.takeDamage(999);
            enemy->takeDamage(999);
            return; // Thoát ngay vì Tàu đã "chết"
        }
    }

    // --- 2.Va chạm Tàu vs Đạn Quái ---
    for (int i = enemyBullets.size() - 1; i >= 0; i--)
    {
        sf::FloatRect bulletBounds = enemyBullets[i].sprite.getGlobalBounds();

        if (playerBounds.intersects(bulletBounds))
        {
            // --- CÓ VA CHẠM! ---

            // 1. Tàu nhận sát thương (đúng bằng damage của viên đạn)
            player.takeDamage(enemyBullets[i].getDamage());

            // 2. Xóa viên đạn đã va chạm
            enemyBullets.erase(enemyBullets.begin() + i);
        }
    }
}
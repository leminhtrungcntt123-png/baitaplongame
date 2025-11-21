#include "../header/CollisionManager.h"
#include <SFML/Graphics.hpp>
#include "../header/Powerup.h"
#include "../header/Player.h"

// --- HÀM 1: ĐẠN TÀU vs QUÁI ---
void CollisionManager::ProcessCollisions(std::vector<Bullet>& bullets,
    std::vector<std::unique_ptr<EnemyBase>>& enemies)
{
    // Dùng vòng lặp "for" ngược
    for (int i = enemies.size() - 1; i >= 0; i--) // "i" LÀ QUÁI (ENEMY)
    {
        for (int j = bullets.size() - 1; j >= 0; j--) // "j" LÀ ĐẠN (BULLET)
        {
            // Lấy hitbox Kẻ Thù (LÀ "ENTITY") -> Dùng .getBounds()
            sf::FloatRect enemyBounds = enemies[i]->getBounds(); // <-- SỬA "j" THÀNH "i"

            // Lấy hitbox Đạn (KHÔNG LÀ "ENTITY") -> Dùng .sprite.
            sf::FloatRect bulletBounds = bullets[j].sprite.getGlobalBounds(); // <-- SỬA "i" THÀNH "j" VÀ THÊM ".sprite"

            // Kiểm tra va chạm
            if (bulletBounds.intersects(enemyBounds))
            {
                // Va chạm (i = Quái, j = Đạn) -> Đã đúng
                enemies[i]->takeDamage(bullets[j].getDamage());
                bullets.erase(bullets.begin() + j);
                break;
            }
        }
    }
}

// --- HÀM 2: TÀU vs VẬT PHẨM ---
bool CollisionManager::ProcessPlayerCollisions(Player& player,
    std::vector<Powerup>& powerups)
{
    sf::FloatRect playerBounds = player.getBounds();

    for (int i = powerups.size() - 1; i >= 0; i--)
    {
        sf::FloatRect powerupBounds = powerups[i].sprite.getGlobalBounds();

        if (playerBounds.intersects(powerupBounds))
        {
            // 1. Ăn đạn thường
            if (powerups[i].type == Powerup::UpgradeGun)
            {
                player.upgradeGun();
            }
            // 2. Ăn Mega (Biến hình)
            else if (powerups[i].type == Powerup::MegaUpgrade)
            {
                player.megaUpgradeGun();
            }
            // --- [BẢN VÁ] 3. Ăn Máu (Hồi phục) ---
            else if (powerups[i].type == Powerup::HealthPack)
            {
                player.heal(3); // Hồi 30 HP (Bạn có thể chỉnh số này tùy ý)
            }

            // Xóa vật phẩm sau khi ăn xong
            powerups.erase(powerups.begin() + i);
            return true;
        }
    }
    return false;
}
// --- HÀM 3: TÀU vs QUÁI / ĐẠN QUÁI ---
void CollisionManager::ProcessPlayerHits(Player& player,
    std::vector<std::unique_ptr<EnemyBase>>& enemies,
    std::vector<Bullet>& enemyBullets)
{
    if (!player.isAlive()) return;

    // Lấy hitbox Tàu (LÀ "ENTITY") -> Dùng .getBounds()
    sf::FloatRect playerBounds = player.getBounds();

    // 1. Va chạm Tàu vs Quái (Đâm nhau)
    for (auto& enemy : enemies)
    {
        // Lấy hitbox Kẻ Thù (LÀ "ENTITY") -> Dùng .getBounds()
        sf::FloatRect enemyBounds = enemy->getBounds(); // <-- SỬA LỖI DÒNG 77

        if (playerBounds.intersects(enemyBounds))
        {
            player.takeDamage(999);
            enemy->takeDamage(999);
            return;
        }
    }

    // 2. Va chạm Tàu vs Đạn Quái
    for (int i = enemyBullets.size() - 1; i >= 0; i--)
    {
        // Lấy hitbox Đạn (KHÔNG LÀ "ENTITY") -> Dùng .sprite.
        sf::FloatRect bulletBounds = enemyBullets[i].sprite.getGlobalBounds();

        if (playerBounds.intersects(bulletBounds))
        {
            player.takeDamage(enemyBullets[i].getDamage());
            enemyBullets.erase(enemyBullets.begin() + i);
        }
    }
}

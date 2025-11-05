#include "Player.h"

// --- HÀM DỰNG (CONSTRUCTOR) ---
// Dùng danh sách khởi tạo để gán các tham chiếu
Player::Player(sf::Texture& playerTexture, sf::Texture& bulletTexture,
    std::vector<Bullet>& bullets)
    : mBulletTextureRef(bulletTexture), // Gán tham chiếu texture
    mPlayerBulletsRef(bullets),        // Gán tham chiếu vector đạn
    mGunLevel(1),                     // Tàu tự quản lý cấp súng
    mMoveSpeed(700.f),               // Tàu tự quản lý tốc độ
    mHp(10),                        // Khởi tạo Hp của tàu  
    mMaxHp(10)
{
    // Tàu tự thiết lập sprite của mình
    mSprite.setTexture(playerTexture);
    mSprite.setScale(2.1f, 2.1f);
}

// Hàm này được gọi 1 lần từ Game.cpp
void Player::setInitialPosition(float x, float y)
{
    mSprite.setPosition(x, y);
}

// --- PHÒNG BAN XỬ LÝ INPUT (SỰ KIỆN) ---
bool Player::handleInput(sf::Event& event, float bulletSpeed)
{
    // Chỉ xử lý logic bắn súng (Event::KeyPressed)
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            // *** TOÀN BỘ CODE BẮN SÚNG CỦA BẠN ĐƯỢC CHUYỂN VÀO ĐÂY ***
            sf::Vector2f playerPos = mSprite.getPosition();
            sf::FloatRect bounds = mSprite.getGlobalBounds();

            // Hướng bay của Tàu (0, -1) = 0 X, -1 Y (đi lên)
            sf::Vector2f dir(0.f, -1.f);
            int damage = 1; // Đạn Tàu gây 1 sát thương

            switch (mGunLevel)
            {
            case 1:
            {
                // Căn vị trí bắn ra ở giữa
                sf::Vector2f pos(playerPos.x + bounds.width / 2.f, playerPos.y);
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, pos, dir, bulletSpeed, damage));
                break;
            }
            case 2:
            {
                // Vị trí trái và phải (cũ)
                sf::Vector2f posLeft(playerPos.x, playerPos.y);
                sf::Vector2f posRight(playerPos.x + bounds.width, playerPos.y);
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, posLeft, dir, bulletSpeed, damage));
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, posRight, dir, bulletSpeed, damage));
                break;
            }
            case 3:
            default:
            {
                // Vị trí giữa
                sf::Vector2f posMid(playerPos.x + bounds.width / 2.f, playerPos.y);
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, posMid, dir, bulletSpeed, damage));

                // Vị trí trái
                sf::Vector2f posLeft(playerPos.x - bounds.width / 4.f, playerPos.y);
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, posLeft, dir, bulletSpeed, damage));

                // Vị trí phải
                sf::Vector2f posRight(playerPos.x + bounds.width + bounds.width / 4.f, playerPos.y);
                mPlayerBulletsRef.push_back(Bullet(mBulletTextureRef, posRight, dir, bulletSpeed, damage));
                break;
            }
            } // kết thúc switch 
            return true;
        }
    }
    return false; // Không bắn
}

// --- PHÒNG BAN CẬP NHẬT LOGIC (TRẠNG THÁI) ---
void Player::update(float deltaTime, float windowWidth)
{
    // *** TOÀN BỘ CODE DI CHUYỂN CỦA BẠN ĐƯỢC CHUYỂN VÀO ĐÂY ***

    // 1. Logic di chuyển (Keyboard::isKeyPressed)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        mSprite.move(-mMoveSpeed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mSprite.move(mMoveSpeed * deltaTime, 0.f);
    }

    // 2. Ngăn tàu ra khỏi màn hình
    sf::FloatRect playerBounds = mSprite.getGlobalBounds();
    if (playerBounds.left < 0.f)
    {
        mSprite.setPosition(0.f, playerBounds.top);
    }
    else if (playerBounds.left + playerBounds.width > windowWidth)
    {
        mSprite.setPosition(windowWidth - playerBounds.width, playerBounds.top);
    }
}

// --- PHÒNG BAN VẼ ---
void Player::draw(sf::RenderWindow& window)
{
    window.draw(mSprite);
}

// --- HÀM TIỆN ÍCH ---
void Player::upgradeGun()
{
    if (mGunLevel < 3)
    {
        mGunLevel++;
    }
}

sf::FloatRect Player::getBounds() const
{
    return mSprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return mSprite.getPosition();
}

// --- TRIỂN KHAI CÁC HÀM HP MỚI ---
void Player::takeDamage(int damage)
{
    mHp -= damage;
    if (mHp < 0)
    {
        mHp = 0;
    }
}

bool Player::isAlive() const
{
    return mHp > 0;
}

int Player::getHP() const
{
    return mHp;
}
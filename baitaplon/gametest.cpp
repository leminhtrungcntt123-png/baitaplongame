#include <SFML/Graphics.hpp>
#include "header/Player.h"
#include "header/Enemy.h"
#include "header/Boss.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector> // Thêm thư viện vector

using namespace sf;
using namespace std;

// Hàm tạo vị trí Y ngẫu nhiên (để tái sử dụng dễ dàng)
float getRandomEnemyY(int windowHeight)
{
    // Giới hạn Y từ 50 đến 250 (nửa trên màn hình, như bạn đã yêu cầu trước đó)
    const int MIN_Y = 50;
    const int MAX_Y = 250;
    return static_cast<float>(rand() % (MAX_Y - MIN_Y + 1) + MIN_Y);
}

int main()
{
    // --- KHỞI TẠO ---
    RenderWindow window(VideoMode(800, 600), "Player + Animation Test");
    window.setFramerateLimit(60);

    // *** KHỞI TẠO BỘ SINH SỐ NGẪU NHIÊN ***
    srand(static_cast<unsigned int>(time(NULL)));
    const float START_X = 800.f + 50.f; // Vị trí X bắt đầu ngoài màn hình

    // Load texture
    Texture playerTexture;
    if (!playerTexture.loadFromFile("model/player2.png"))
        return -1;
    Texture enermy1Texture;
    if (!enermy1Texture.loadFromFile("model/cc.png"))
        return -1;
    Texture enermy2Texture;
    if (!enermy2Texture.loadFromFile("model/gian.png"))
        return -1;
    Texture enermy3Texture;
    if (!enermy3Texture.loadFromFile("model/ONG.png"))
        return -1;
    Texture enermy4Texture;
    if (!enermy4Texture.loadFromFile("model/ruoi.png"))
        return -1;
    Texture BossTexture;
    if (!BossTexture.loadFromFile("model/boss.png"))
        return -1;

    // TẠO PLAYER
    Player player(&playerTexture, Vector2f(400.f, 300.f), Vector2f(360, 266));
    Boss boss(&BossTexture, Vector2f(300, 100), 200.f, Vector2f(285, 266));
    // -----------------------------------------------------
    // *** KHỞI TẠO VÀ QUẢN LÝ NHIỀU ENEMY BẰNG VECTOR ***
    // -----------------------------------------------------

    vector<Enemy> enemies;

    // Enemy 1 (Sử dụng texture cc.png)
    enemies.emplace_back(&enermy1Texture,
                         Vector2f(START_X, getRandomEnemyY(window.getSize().y)),
                         100.f, Vector2f(260, 300));

    // Enemy 2 (Sử dụng texture gian.png)
    enemies.emplace_back(&enermy2Texture,
                         Vector2f(START_X + 150.f, getRandomEnemyY(window.getSize().y)), // Dịch X một chút để chúng không spawn cùng một hàng dọc
                         100.f, Vector2f(290, 300));

    // Enemy 3 (Thêm một con nữa để kiểm tra)
    enemies.emplace_back(&enermy3Texture,
                         Vector2f(START_X + 300.f, getRandomEnemyY(window.getSize().y)),
                         100.f, Vector2f(290, 300));
    enemies.emplace_back(&enermy4Texture,
                         Vector2f(START_X + 300.f, getRandomEnemyY(window.getSize().y)),
                         100.f, Vector2f(225, 300));

    Clock clock;

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // --- ĐIỀU KHIỂN ---
        Vector2f velocity(0.f, 0.f);
        float speed = 180.f; // Đã giảm tốc độ (như tôi đề xuất trước đó)

        if (Keyboard::isKeyPressed(Keyboard::Right))
            velocity.x = speed;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            velocity.x = -speed;
        if (Keyboard::isKeyPressed(Keyboard::Up))
            velocity.y = -speed;
        if (Keyboard::isKeyPressed(Keyboard::Down))
            velocity.y = speed;

        player.setVelocity(velocity);
        // --- CẬP NHẬT ---
        player.update(dt, window);
        boss.update(dt, window, player.getPosition());

        // Cập nhật tất cả enemies
        for (Enemy &enemy : enemies)
        {
            enemy.update(dt, window);
        }

        // --- VẼ ---
        window.clear();
        player.draw(window);
        boss.draw(window);
        // Vẽ tất cả enemies
        for (Enemy &enemy : enemies)
        {
            enemy.draw(window);
        }

        window.display();
    }

    return 0;
}
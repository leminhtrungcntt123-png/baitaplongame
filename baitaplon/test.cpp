#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace sf;
using namespace std;

// ---------------- CLASS AMMO ----------------
class Ammo {
public:
    Sprite sprite;
    float speed;

    Ammo(float startX, float startY, sf::Texture& texture) {
        speed = 8.0f;
        sprite.setTexture(texture);

        // --- Ảnh có 3 viên đạn xếp ngang ---
        int totalFrames = 3;
        int frameWidth = texture.getSize().x / totalFrames;
        int frameHeight = texture.getSize().y;
        int bulletIndex = 1; // 0 = trái, 1 = giữa, 2 = phải

        sprite.setTextureRect(IntRect(
            bulletIndex * frameWidth,
            0,
            frameWidth,
            frameHeight
        ));

        sprite.setScale(0.4f, 0.4f); // phóng to hoặc thu nhỏ
        float scaledW = sprite.getGlobalBounds().width;
        sprite.setPosition(startX - scaledW / 2.f, startY);
    }

    void update() { sprite.move(0, -speed); }

    bool isOffScreen() {
        return sprite.getPosition().y + sprite.getGlobalBounds().height < 0;
    }

    void draw(RenderWindow& window) { window.draw(sprite); }
};

// ---------------- MAIN ----------------
int main() {
    RenderWindow window(VideoMode(800, 600), "Ban dan SFML - Bullet Transparency");
    window.setFramerateLimit(60);

    // --- Load ảnh và loại bỏ nền trắng ---
    Image bulletImg;
    if (!bulletImg.loadFromFile("bullet.png")) {
        cerr << "ERROR: Failed to load bullet.png!" << endl;
        return -1;
    }

    // Loại bỏ nền trắng → trong suốt
    bulletImg.createMaskFromColor(Color::White);

    Texture bulletTexture;
    if (!bulletTexture.loadFromImage(bulletImg)) {
        cerr << "ERROR: Failed to load texture from image!" << endl;
        return -1;
    }

    // --- Player ---
    const float playerW = 40.f, playerH = 80.f;
    RectangleShape player(Vector2f(playerW, playerH));
    player.setFillColor(Color::Blue);
    player.setPosition(380, 500);

    vector<Ammo> bullets;
    Clock shootTimer;
    float shootInterval = 0.1f, moveSpeed = 20.0f;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // --- Di chuyển ---
        if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0)
            player.move(-moveSpeed, 0);

        if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x < 800 - playerW)
            player.move(moveSpeed, 0);

        // --- Bắn tự động ---
        if (shootTimer.getElapsedTime().asSeconds() >= shootInterval) {
            float spawnX = player.getPosition().x + playerW / 2.f;
            float spawnY = player.getPosition().y - 10.f;
            bullets.emplace_back(spawnX, spawnY, bulletTexture);
            shootTimer.restart();
        }

        // --- Cập nhật đạn ---
        for (auto& b : bullets) b.update();

        // --- Xóa đạn bay ra khỏi màn hình ---
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](Ammo& b) { return b.isOffScreen(); }), bullets.end());

        // --- Vẽ ---
        window.clear(Color::Black);
        window.draw(player);
        for (auto& b : bullets) b.draw(window);
        window.display();
    }

    return 0;
}

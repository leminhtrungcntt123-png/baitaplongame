#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;

// ---------------- CLASS AMMO ----------------
class Ammo {
public:
    RectangleShape shape;
    float speed;

    Ammo(float startX, float startY) {
        shape.setSize(Vector2f(10, 4));
        shape.setFillColor(Color::Red);
        shape.setPosition(startX, startY);
        speed = 8.0f;
    }

    void update() {
        shape.move(speed, 0);
    }

    bool isOffScreen() {
        return shape.getPosition().x > 800;
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    RenderWindow window(VideoMode(800, 600), "Ban dan SFML");
    window.setFramerateLimit(60);

    CircleShape player(20);
    player.setFillColor(Color::Green);
    player.setPosition(100, 300);

    vector<Ammo> bullets;
    Clock shootTimer;
    float shootInterval = 0.5f;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Di chuyển bằng phím mũi tên lên xuống
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (player.getPosition().y > 0) {
                player.move(0, -5);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (player.getPosition().y < 560) {
                player.move(0, 5);
            }
        }

        // Tự động bắn đạn
        if (shootTimer.getElapsedTime().asSeconds() >= shootInterval) {
            bullets.push_back(Ammo(player.getPosition().x + 40, player.getPosition().y + 15));
            shootTimer.restart();
        }

        // Cập nhật đạn
        for (auto& b : bullets) {
            b.update();
        }

        // Xóa đạn bay ra khỏi màn hình
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](Ammo& b) { return b.isOffScreen(); }), bullets.end());

        window.clear();
        window.draw(player);
        for (auto& b : bullets) {
            b.draw(window);
        }
        window.display();
    }

    return 0;
}
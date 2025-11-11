#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test SFML");
    sf::CircleShape shape(20.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100, 300);
    
    sf::Clock clock;
    float moveSpeed = 5.0f;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Di chuyển với phím mũi tên
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            shape.move(0, -moveSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            shape.move(0, moveSpeed);

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
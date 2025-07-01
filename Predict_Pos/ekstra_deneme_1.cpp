#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Basit Çizgi");

    std::vector<float> values = { 10, 50, 30, 80, 60 };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        sf::VertexArray line(sf::LineStrip, values.size());

        for (size_t i = 0; i < values.size(); ++i) {
            line[i].position = sf::Vector2f(100 + i * 100, 500 - values[i]);
            line[i].color = sf::Color::Red;
        }

        window.draw(line);
        window.display();
    }

    return 0;
}
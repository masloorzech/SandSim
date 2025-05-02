#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 25

void draw_map(sf::RenderWindow &window, const std::vector<std::vector<bool>> &map, size_t width, size_t height) {
    sf::RectangleShape tile(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
    tile.setOutlineThickness(1);
    tile.setOutlineColor(sf::Color::White);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            tile.setPosition(sf::Vector2f (x* PIXEL_SIZE, y * PIXEL_SIZE));
            if (map[y][x]) {
                tile.setFillColor(sf::Color::White);
            } else {
                tile.setFillColor(sf::Color::Black);
            }
            window.draw(tile);
        }
    }
}



int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SandSim");

    const size_t width = 20;
    const size_t height = 20;
    std::vector<std::vector<bool>> map(height, std::vector<bool>(width)); // [y][x]

    map[0][10] = true;

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        draw_map(window, map, width, height);



        window.display();
    }

    return 0;
}

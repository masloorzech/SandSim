#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 25

typedef std::vector<std::vector<bool>> map_t;

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

void apply_physics(map_t& map){
    std::vector<std::vector<bool>> map_copy = map; // głęboka kopia startowa

    for (int y = map.size() - 2; y >= 0; y--) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] && !map[y+1][x]) {
                map_copy[y][x] = false;
                map_copy[y+1][x] = true;
            }
        }
    }

    map = map_copy;
}

void place_sand(map_t& map, const sf::RenderWindow& window){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    int grid_x = mousePos.x / PIXEL_SIZE;
    int grid_y = mousePos.y / PIXEL_SIZE;

    if (grid_x >= 0 && grid_x < map[0].size() && grid_y >= 0 && grid_y < map.size()) {
        map[grid_y][grid_x] = true;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SandSim");

    const size_t width = 20;
    const size_t height = 20;
    std::vector<std::vector<bool>> map(height, std::vector<bool>(width)); // [y][x]

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                place_sand(map,window);
            }

            }

        window.clear();

        apply_physics(map);

        draw_map(window, map, width, height);

        window.display();
    }

    return 0;
}

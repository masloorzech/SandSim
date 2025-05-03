#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 5
typedef std::vector<std::vector<bool>> map_t;

void draw_map(sf::RenderWindow &window, sf::Vector2f offset,const std::vector<std::vector<bool>> &map, size_t width, size_t height) {


    sf::RectangleShape frame(sf::Vector2f(width*PIXEL_SIZE, height*PIXEL_SIZE));
    frame.setOutlineThickness(1);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Black);
    frame.setPosition(sf::Vector2f(offset.x+0,offset.y+0));

    window.draw(frame);

    sf::RectangleShape tile(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            tile.setPosition(sf::Vector2f (offset.x + (x* PIXEL_SIZE), (offset.y + y * PIXEL_SIZE)));
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
    std::vector<std::vector<bool>> map_copy = map;

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

void place_sand(const sf::RenderWindow& window, map_t& map, sf::Vector2f offset){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    int grid_x = (mousePos.x - offset.x) / PIXEL_SIZE;
    int grid_y = (mousePos.y - offset.y) / PIXEL_SIZE;

    if (grid_x >= 0 && grid_x < map[0].size() && grid_y >= 0 && grid_y < map.size()) {
        map[grid_y][grid_x] = true;
    }
}

int main() {

    sf::Vector2f window_size = sf::Vector2f(800,600);
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");

    const size_t width = 128;
    const size_t height = 64;
    std::vector<std::vector<bool>> map(height, std::vector<bool>(width)); // [y][x]

    auto draw_map_offset = sf::Vector2f(static_cast<unsigned int >(window_size.x/2 - (width*PIXEL_SIZE)/2), static_cast<unsigned int >(window_size.y/2 - (height*PIXEL_SIZE)/2));

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                place_sand(window, map,draw_map_offset);
            }

            }

        window.clear();

        apply_physics(map);

        draw_map(window, draw_map_offset,map, width, height);

        window.display();
    }

    return 0;
}

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 5

#define RADIUS 2

#define SAND_COLOR sf::Color(242,210,169)

struct tile{
    bool value;
    sf::Color color;
};

typedef std::vector<std::vector<tile>> map_t;

void draw_map(sf::RenderWindow &window, sf::Vector2f offset,const map_t &map, size_t width, size_t height) {


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
            if (map[y][x].value) {
                tile.setFillColor(map[y][x].color);
            } else {
                tile.setFillColor(sf::Color::Black);
            }
            window.draw(tile);
        }
    }

}

void apply_physics(map_t& map){
    map_t map_copy = map;

    for (int y = map.size() - 2; y >= 0; y--) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x].value && !map[y+1][x].value) {
                map_copy[y][x].value = false;
                map_copy[y+1][x].value = true;
                map_copy[y+1][x].color = map_copy[y][x].color;
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

        for (int dy = -RADIUS; dy <= RADIUS; ++dy) {
            for (int dx = -RADIUS; dx <= RADIUS; ++dx) {
                int x = grid_x + dx;
                int y = grid_y + dy;

                if (x >= 0 && x < map[0].size() && y >= 0 && y < map.size()) {
                    if (dx * dx + dy * dy <= RADIUS * RADIUS) {
                        map[y][x].value = true;
                        map[y][x].color = SAND_COLOR;;
                    }
                }
            }
        }
    }
}

int main() {

    sf::Vector2f window_size = sf::Vector2f(800,600);
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");

    const size_t width = 128;
    const size_t height = 64;
    map_t map(height, std::vector<tile>(width));

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

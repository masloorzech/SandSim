#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 1

#define RADIUS 4

#define SAND_COLOR sf::Color(242,210,169)

struct tile{
    bool value;
    sf::Color color;
};

typedef std::vector<std::vector<tile>> map_t;

void draw_map(sf::RenderWindow& window, sf::Vector2f offset, const map_t& map, size_t width, size_t height) {

    sf::RectangleShape frame(sf::Vector2f(width * PIXEL_SIZE, height * PIXEL_SIZE));
    frame.setOutlineThickness(2);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Transparent);
    frame.setPosition(sf::Vector2f(offset.x, offset.y));

    window.draw(frame);

    sf::VertexArray vertices(sf::PrimitiveType::Triangles, width * height * 6);  // 6 vertices for each quad (2 triangles)

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            size_t index = (y * width + x) * 6;
            sf::Color color = map[y][x].value ? map[y][x].color : sf::Color::Black;

            float xPos = offset.x + (x * PIXEL_SIZE);
            float yPos = offset.y + (y * PIXEL_SIZE);

            vertices[index].position = sf::Vector2f(xPos, yPos);
            vertices[index + 1].position = sf::Vector2f(xPos + PIXEL_SIZE, yPos);
            vertices[index + 2].position = sf::Vector2f(xPos, yPos + PIXEL_SIZE);

            vertices[index].color = color;
            vertices[index + 1].color = color;
            vertices[index + 2].color = color;

            vertices[index + 3].position = sf::Vector2f(xPos + PIXEL_SIZE, yPos);
            vertices[index + 4].position = sf::Vector2f(xPos + PIXEL_SIZE, yPos + PIXEL_SIZE);
            vertices[index + 5].position = sf::Vector2f(xPos, yPos + PIXEL_SIZE);

            vertices[index + 3].color = color;
            vertices[index + 4].color = color;
            vertices[index + 5].color = color;
        }
    }

    window.draw(vertices);
}

void apply_physics(map_t& map) {
    for (int y = map.size() - 2; y >= 0; y--) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x].value) {
                // Move down
                if (!map[y+1][x].value) {
                    map[y][x].value = false;
                    map[y+1][x].value = true;
                    map[y+1][x].color = map[y][x].color;
                }
                else if (x+1 < map[y].size() && !map[y+1][x+1].value) {
                    map[y][x].value = false;
                    map[y+1][x+1].value = true;
                    map[y+1][x+1].color = map[y][x].color;
                }
                else if (x-1 >= 0 && !map[y+1][x-1].value) {
                    map[y][x].value = false;
                    map[y+1][x-1].value = true;
                    map[y+1][x-1].color = map[y][x].color;
                }
            }
        }
    }
}

void place_sand(const sf::RenderWindow& window, map_t& map, sf::Vector2f offset) {

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
                        map[y][x].color = SAND_COLOR;
                    }
                }
            }
        }
    }
}

int main() {

    sf::Vector2f window_size = sf::Vector2f(800,600);
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");

    const size_t width = 512;
    const size_t height = 512;

    map_t map(height, std::vector<tile>(width));

    auto draw_map_offset = sf::Vector2f(static_cast<unsigned int >(window_size.x/2 - (width*PIXEL_SIZE)/2), static_cast<unsigned int >(window_size.y/2 - (height*PIXEL_SIZE)/2));

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
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

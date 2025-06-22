//
// Created by anton on 19.06.2025.
//

#include "SandMap.h"


SandMap::SandMap(const sf::Vector2u map_size,sf::RenderWindow &window): window(window), width(map_size.x), height(map_size.y){

    map = std::vector(map_size.x, std::vector<Tile>(map_size.y));

    for (unsigned int x = 0; x < map_size.x; ++x) {
        for (unsigned int y = 0; y < map_size.y; ++y) {
            map[x][y].setValue(TileType::AIR);
            map[x][y].setColor(sf::Color::Black);
        }
    }
}

void SandMap::clear(){
    for (auto& row : map) {
        for (auto& tile : row) {
            tile.setColor(sf::Color::Black);
            tile.setValue(TileType::AIR);
        }
    }
}

void SandMap::changeType(TileType old_type, TileType new_type) {
    for (auto &row: map) {
        for (auto &col: row) {
            if (col.getValue() == old_type) {
                col.setValue(new_type);
            }
        }
    }
}

void SandMap::draw(const sf::Vector2f offset, const sf::Color &background_color) {
    sf::Image image(sf::Vector2u(width, height), background_color);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (map[y][x].getValue() != TileType::AIR) {
                image.setPixel(sf::Vector2u(x, y), map[y][x].getColor());
            }
        }
    }

    sf::Texture texture;

    texture.loadFromImage(image);

    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f(static_cast<float>(Config::PIXEL_SIZE), static_cast<float>(Config::PIXEL_SIZE)));
    sprite.setPosition(offset);

    sf::RectangleShape frame(sf::Vector2f(static_cast<float>(width) * Config::PIXEL_SIZE, static_cast<float>(height) * Config::PIXEL_SIZE));
    frame.setOutlineThickness(Config::PIXEL_SIZE);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Transparent);
    frame.setPosition(offset);

    window.draw(sprite);
    window.draw(frame);
}

void SandMap::applyPhysics() {

    for (int y = height - 2; y >= 0; --y) {
        std::vector<int> x_indices(width);
        std::iota(x_indices.begin(), x_indices.end(), 0);
        std::shuffle(x_indices.begin(), x_indices.end(), fast_rng);

        for (int x : x_indices) {
            if (map[y][x].getValue() == TileType::SAND) {
                if (y + 1 < height && map[y + 1][x].getValue() == TileType::AIR) {
                    map[y + 1][x] = map[y][x];
                    map[y][x].setValue(TileType::AIR);
                    map[y][x].setColor(sf::Color::Black);
                }
                else if (y + 1 < height) {
                    std::vector<int> directions;

                    if (x > 0 && map[y + 1][x - 1].getValue() == TileType::AIR) {
                        directions.push_back(-1);
                    }
                    if (x + 1 < width && map[y + 1][x + 1].getValue() == TileType::AIR) {
                        directions.push_back(1);
                    }

                    if (!directions.empty()) {
                        int direction = directions[fast_rng() % directions.size()];
                        int new_x = x + direction;

                        map[y + 1][new_x] = map[y][x];
                        map[y][x].setValue(TileType::AIR);
                        map[y][x].setColor(sf::Color::Black);
                    }
                }
            }
        }
    }
}

void SandMap::save(const std::string &filename, sf::Color background_color) {

    sf::Image image(sf::Vector2u(width, height), background_color);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (map[y][x].getValue() != TileType::AIR) {
                image.setPixel(sf::Vector2u(x, y), map[y][x].getColor());
            }
        }
    }
    image.saveToFile(filename);
}


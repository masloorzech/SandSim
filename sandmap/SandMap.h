//
// Created by anton on 19.06.2025.
//

#ifndef SANDMAP_H
#define SANDMAP_H

#include <filesystem>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Config.h"

class SandMap {
    std::vector<std::vector<Tile>> map;
    unsigned int width, height;
    sf::RenderWindow& window;

public:
    SandMap(sf::Vector2u map_size,sf::RenderWindow &window);

    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

    const std::vector<Tile>& operator[](unsigned int y) const {
        return map[y];
    }
    std::vector<Tile>& operator[](unsigned int y) {
        return map[y];
    }

    void clear();

    void changeType(TileType old_type, TileType new_type);

    void applyPhysics();

    void draw(sf::Vector2f offset, const sf::Color &background_color);

    void save(const std::string &filename, sf::Color background_color);

};



#endif //SANDMAP_H

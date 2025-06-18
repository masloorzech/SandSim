//
// Created by anton on 19.06.2025.
//

#ifndef TILE_H
#define TILE_H

#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <Windows.h>

enum class TileType {
    AIR = 0,
    SAND = 1,
    SOLID = 2
};

class Tile {

    TileType value = TileType::AIR;
    sf::Color color = sf::Color::Black;

public:
    Tile() = default;

    TileType getValue() const { return value; }
    sf::Color getColor() const { return color; }

    void setValue(TileType newValue) { value = newValue; }
    void setColor(const sf::Color& newColor) { color = newColor; }

    bool isEmpty() const { return value == TileType::AIR; }
    bool isSand() const { return value == TileType::SAND; }
    bool isSolid() const { return value == TileType::SOLID; }
};



#endif //TILE_H

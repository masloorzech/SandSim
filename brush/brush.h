//
// Created by anton on 22.06.2025.
//

#ifndef BRUSH_H
#define BRUSH_H

#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <Windows.h>
#include "Tile/Tile.h"
#include "sandmap/SandMap.h"
#include "components/Button.h"
#include "components/Slider.h"

void use_brush(const sf::RenderWindow &window, SandMap &map, sf::Vector2f offset,  int radius,  sf::Color color,  TileType draw_with);

#endif //BRUSH_H

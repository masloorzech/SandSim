//
// Created by anton on 22.06.2025.
//

#include "brush.h"

void use_brush(const sf::RenderWindow &window, SandMap &map,  sf::Vector2f offset,  int radius,  sf::Color color,  TileType draw_with) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const int grid_x = (mousePos.x - static_cast<int>(offset.x)) / Config::PIXEL_SIZE;
    const int grid_y = (mousePos.y - static_cast<int>(offset.y)) / Config::PIXEL_SIZE;

    if (grid_x < 0 || grid_x >= map[0].size() || grid_y < 0 || grid_y >= map.getHeight()) return;

    std::vector<std::pair<int, int>> points;
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy > radius * radius) continue;

            int x = grid_x + dx;
            int y = grid_y + dy;
            if (x < 0 || x >= map[0].size() || y < 0 || y >= map.getHeight()) continue;

            points.emplace_back(x, y);
        }
    }

    std::sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    for (const auto& [x, y] : points) {
        Tile &t = map[y][x];

        switch (draw_with) {
            case TileType::SAND:
                if (t.getValue() != TileType::SOLID && t.getValue()!= TileType::SAND) {
                    t.setValue(TileType::SAND);
                    t.setColor(color);
                }
            break;

            case TileType::AIR:
                if (t.getValue() != TileType::AIR) {
                    t.setValue(TileType::AIR);
                    t.setColor(sf::Color::Black);
                }
            break;

            case TileType::SOLID:
                if (t.getValue() == TileType::AIR) {
                    t.setValue(TileType::SOLID);
                    t.setColor(color);
                }
            break;
        }
    }
}
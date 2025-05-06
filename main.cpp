#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>

#include "components/Button.h"
#include "components/Slider.h"

#define SLIDERS_WIDTH 150
#define SLIDERS_HEIGHT 30
#define SLIDERS_X_OFFSET 10

#define PIXEL_SIZE 1

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 840

#define MAP_WIDTH 800
#define MAP_HEIGHT 800

#define IMG_FOLDER "screenshots"

enum class TileType {
    AIR = 0,
    SAND = 1,
    SOLID = 2
};

struct tile {
    TileType value = TileType::AIR;
    sf::Color color = sf::Color::Black;
};

using map_t = std::vector<std::vector<tile>>;

std::minstd_rand fast_rng(std::random_device{}());

sf::RenderWindow init_screen(const sf::Vector2f window_size) {
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");
    return window;
}

map_t init_map(const sf::Vector2u map_size) {
    map_t map(map_size.x, std::vector<tile>(map_size.y));

    for (unsigned int x = 0; x < map_size.x; ++x) {
        for (unsigned int y = 0; y < map_size.y; ++y) {
            map[x][y].value = TileType::AIR;
            map[x][y].color = sf::Color::Black;
        }
    }
    return map;
}

std::vector<Slider> init_color_sliders(const sf::Vector2f position, const int x_spacing, const int y_spacing, const sf::Font &font) {
    auto R_slider = Slider(position, sf::Vector2f(SLIDERS_WIDTH,SLIDERS_HEIGHT), 0, 255, font);
    R_slider.set_text("Red Value");
    R_slider.set_slider_color(sf::Color::Red);
    R_slider.set_slider_value(128);

    auto G_slider = Slider(sf::Vector2f(static_cast<float>(static_cast<int>(position.x) + x_spacing), static_cast<float>(static_cast<int>(position.y) + y_spacing)),
                             sf::Vector2f(SLIDERS_WIDTH,SLIDERS_HEIGHT), 0, 255, font);
    G_slider.set_text("Green Value");
    G_slider.set_slider_color(sf::Color::Green);
    G_slider.set_slider_value(128);

    auto B_slider = Slider(sf::Vector2f(static_cast<float>(static_cast<int>(position.x) + x_spacing), static_cast<float>(static_cast<int>(position.y) + 2 * y_spacing)),
                             sf::Vector2f(SLIDERS_WIDTH,SLIDERS_HEIGHT), 0, 255, font);
    B_slider.set_text("Blue Value");
    B_slider.set_slider_color(sf::Color::Blue);
    B_slider.set_slider_value(128);

    auto A_slider = Slider(sf::Vector2f(static_cast<float>(static_cast<int>(position.x) + x_spacing), static_cast<float>(static_cast<int>(position.y) + 3 * y_spacing)),
                             sf::Vector2f(SLIDERS_WIDTH,SLIDERS_HEIGHT), 0, 255, font);
    A_slider.set_text("Alpha Value");
    A_slider.set_slider_color(sf::Color::Transparent);
    A_slider.set_slider_value(128);

    auto brush_color_sliders = std::vector<Slider>();

    brush_color_sliders.push_back(R_slider);
    brush_color_sliders.push_back(G_slider);
    brush_color_sliders.push_back(B_slider);
    brush_color_sliders.push_back(A_slider);

    return brush_color_sliders;
}

bool mouse_in_bounds(const sf::RectangleShape& bounds, const sf::RenderWindow &window) {
    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    return bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
}

bool save_map_as_image(const map_t &map, const std::string &filename) {
    if (map.empty() || map[0].empty()) return false;

    const size_t height = map.size();
    const size_t width = map[0].size();

    sf::Image image(sf::Vector2u(width, height), sf::Color::Black);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (map[y][x].value == TileType::SAND) {
                image.setPixel(sf::Vector2u(x, y), map[y][x].color);
            }
        }
    }

    return image.saveToFile(filename);
}

sf::Font load_font() {
    auto pixel_font = sf::Font();
    if (!pixel_font.openFromFile("assets/fonts/Pixel-Regular.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        exit(EXIT_FAILURE);
    }
    return pixel_font;
}

sf::RectangleShape get_map_screen_bounds(const sf::Vector2u map_size, const sf::Vector2f screen_map_offset) {
    auto map_rect = sf::RectangleShape(sf::Vector2f(static_cast<float>(map_size.x * PIXEL_SIZE), static_cast<float>(map_size.y * PIXEL_SIZE)));
    map_rect.setPosition(screen_map_offset);
    return map_rect;
}

sf::RectangleShape init_preview_screen(sf::Vector2f position, sf::Vector2f size, sf::Color color) {
    sf::RectangleShape color_preview(size);

    color_preview.setOutlineColor(sf::Color::White);

    color_preview.setFillColor(color);
    color_preview.setOutlineThickness(1);
    color_preview.setPosition(position);

    return color_preview;
}

Slider init_brush_size_slider(const sf::Vector2f position, const sf::Font &font) {

    auto slider = Slider(position, sf::Vector2f(SLIDERS_WIDTH,SLIDERS_HEIGHT), 0, 64, font);
    slider.set_text("Brush Size");
    slider.set_slider_color(sf::Color(0, 102, 51));
    slider.set_slider_value(10);

    return slider;
}

LatchingButton init_latching_button(const std::string& text, const sf::Vector2f position, const sf::Font &font) {
    auto button = LatchingButton(position, sf::Vector2f(SLIDERS_WIDTH/2.0 - 5,SLIDERS_HEIGHT), sf::Color::Black, font);
    button.set_new_button_hover_color(sf::Color(0, 102, 51));
    button.set_new_button_pressed_color(sf::Color(0, 66, 33));
    button.set_text(text);
    return button;
}

MomentaryButton init_momentary_button(const std::string &text, const sf::Vector2f position, const sf::Font &font) {
    auto button = MomentaryButton(position, sf::Vector2f(SLIDERS_WIDTH/2.0 - 5,SLIDERS_HEIGHT), sf::Color::Black, font);
    button.set_new_button_hover_color(sf::Color(0, 102, 51));
    button.set_new_button_pressed_color(sf::Color(0, 66, 33));
    button.set_text(text);
    return button;
}

sf::Color get_color_from_sliders(const std::vector<Slider>& brush_color_sliders) {
    return sf::Color(brush_color_sliders[0].get_slider_value(), brush_color_sliders[1].get_slider_value(),brush_color_sliders[2].get_slider_value(),brush_color_sliders[3].get_slider_value());
}

bool left_mouse_button_pressed() {
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

bool right_mouse_button_pressed() {
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
}

void handle_reset_button(const bool state, map_t& map, std::vector<Slider>& brush_color_sliders) {
    if (state) {
        for (auto &row: map) {
            for (auto &col: row) {
                col.color = sf::Color::Black;
                col.value = TileType::AIR;
            }
        }
        for (auto &brush_color_slider: brush_color_sliders) {
            brush_color_slider.set_slider_value(127);
        }
    }
}

void handle_solid_button(const bool state, TileType& element) {
    if (state) {
        element = TileType::SOLID;
    } else {
        element = TileType::SAND;
    }
}

void handle_unsolidify_button(const bool state, map_t& map) {
    if (state) {
        for (auto &row: map) {
            for (auto &col: row) {
                if (col.value == TileType::SOLID) {
                    col.value = TileType::SAND;
                }
            }
        }
    }
}

void update_sliders_colors(std::vector<Slider>& brush_color_sliders) {
    brush_color_sliders[0].set_slider_color(sf::Color(brush_color_sliders[0].get_slider_value(), 0, 0, 255));
    brush_color_sliders[1].set_slider_color(sf::Color(0, brush_color_sliders[1].get_slider_value(), 0, 255));
    brush_color_sliders[2].set_slider_color(sf::Color(0, 0, brush_color_sliders[2].get_slider_value(), 255));
    brush_color_sliders[3].set_slider_color(sf::Color(brush_color_sliders[3].get_slider_value(),brush_color_sliders[3].get_slider_value(),brush_color_sliders[3].get_slider_value(), 255));
}

__attribute__((unused))
void draw_map(sf::RenderWindow &window, const sf::Vector2f offset, const map_t &map, const size_t width, const size_t height) {
    sf::RectangleShape frame(sf::Vector2f(static_cast<float>(width) * PIXEL_SIZE, static_cast<float>(height) * PIXEL_SIZE));
    frame.setOutlineThickness(PIXEL_SIZE);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Transparent);
    frame.setPosition(sf::Vector2f(offset.x, offset.y));

    window.draw(frame);

    sf::VertexArray vertices(sf::PrimitiveType::Triangles, width * height * 6);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const size_t index = (y * width + x) * 6;
            const sf::Color color = map[y][x].value != TileType::AIR ? map[y][x].color : sf::Color::Black;

            const float xPos = offset.x + static_cast<float>(x * PIXEL_SIZE);
            const float yPos = offset.y + static_cast<float>(y * PIXEL_SIZE);

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

void draw_map_as_sprite(sf::RenderWindow &window, const sf::Vector2f offset, const map_t &map, const size_t width, const size_t height) {
    sf::Image image(sf::Vector2u(width, height), sf::Color::Black);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (map[y][x].value != TileType::AIR) {
                image.setPixel(sf::Vector2u(x, y), map[y][x].color);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f(static_cast<float>(PIXEL_SIZE), static_cast<float>(PIXEL_SIZE)));
    sprite.setPosition(offset);

    sf::RectangleShape frame(sf::Vector2f(static_cast<float>(width) * PIXEL_SIZE, static_cast<float>(height) * PIXEL_SIZE));
    frame.setOutlineThickness(PIXEL_SIZE);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Transparent);
    frame.setPosition(offset);

    window.draw(sprite);
    window.draw(frame);
}

void try_move_tile(map_t &map, const int x, const int y, const int new_x, const int new_y) {
    if (new_x >= 0 && new_x < map[0].size() && new_y >= 0 && new_y < map.size() && map[new_y][new_x].value == TileType::AIR) {
        map[new_y][new_x] = map[y][x];
        map[y][x].value = TileType::AIR;
    }
}

void apply_physics(map_t &map) {
    const int width = static_cast<int>(map.size() - 2);
    const int height = static_cast<int>(map[0].size());

    for (int y = width; y >= 0; --y) {
        for (int x = 0; x <height; ++x) {
            if (map[y][x].value == TileType::SAND) {
                try_move_tile(map, x, y, x, y +1);

                if (fast_rng() % 100 < 50) {
                    try_move_tile(map, x, y, x + 1, y+1);
                } else {
                    try_move_tile(map, x, y, x - 1, y +1);
                }

            }
        }
    }
}

void use_brush(const sf::RenderWindow &window, map_t &map, const sf::Vector2f offset, const int radius, const sf::Color color, const TileType draw_with) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const int grid_x = (mousePos.x - static_cast<int>(offset.x)) / PIXEL_SIZE;
    const int grid_y = (mousePos.y - static_cast<int>(offset.y)) / PIXEL_SIZE;

    if (grid_x < 0 || grid_x >= map[0].size() || grid_y < 0 || grid_y >= map.size()) return;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy > radius * radius) continue;

            int x = grid_x + dx;
            int y = grid_y + dy;
            if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size()) continue;

            tile &t = map[y][x];


            switch (draw_with) {
                case TileType::SAND:
                    if (t.value != TileType::SAND && t.value != TileType::SOLID) {
                        t.value = draw_with;
                        t.color = color;
                    }
                    break;
                case TileType::AIR:
                    if (t.value == TileType::SAND || t.value == TileType::SOLID) {
                        t.value = draw_with;
                        t.color = color;
                    }
                    break;
                case TileType::SOLID:
                    if (t.value == TileType::AIR) {
                        t.value = draw_with;
                        t.color = color;
                    }
                    break;
            }
        }
    }
}

std::string generate_filename() {

    const std::time_t now = std::time(nullptr);
    const std::tm *tm_ptr = std::localtime(&now);

    std::ostringstream ss;
    ss << IMG_FOLDER << "/screen_"
            << std::put_time(tm_ptr, "%Y-%m-%d_%H-%M-%S")
            << ".png";

    return ss.str();
}

int main() {
    std::filesystem::create_directories(IMG_FOLDER);

    auto window_size = sf::Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT);

    auto map_size = sf::Vector2u(MAP_WIDTH,MAP_HEIGHT);

    auto map = init_map(map_size);

    auto draw_map_offset = sf::Vector2f(static_cast<float>((window_size.x/2.0 - (map_size.x * PIXEL_SIZE)/2.0)),static_cast<float>((window_size.y/2.0 - (map_size.y * PIXEL_SIZE)/2.0)));

    auto pixel_font = load_font();

    auto window_map_bounds = get_map_screen_bounds(
        map_size,
        draw_map_offset);

    auto brush_size_slider = init_brush_size_slider(sf::Vector2f(SLIDERS_X_OFFSET,static_cast<float>(static_cast<int>(draw_map_offset.y))),pixel_font);

    auto brush_color_sliders = init_color_sliders(sf::Vector2f(SLIDERS_X_OFFSET,250),0, 50,pixel_font);

    sf::RenderWindow window = init_screen(window_size);

    auto start_color = sf::Color(brush_color_sliders[0].get_slider_value(), brush_color_sliders[1].get_slider_value(),
                                 brush_color_sliders[2].get_slider_value(), brush_color_sliders[3].get_slider_value());

    auto color_preview_screen = init_preview_screen(sf::Vector2f(SLIDERS_X_OFFSET, 80),sf::Vector2f(SLIDERS_WIDTH, SLIDERS_WIDTH), start_color);

    auto solid_button = init_latching_button("Solid", sf::Vector2f(SLIDERS_X_OFFSET, 450), pixel_font);

    auto colorful_button = init_latching_button("Colorful", sf::Vector2f(SLIDERS_X_OFFSET + SLIDERS_WIDTH/2.0 + 5, 450),pixel_font);

    auto reset_button = init_momentary_button("Clear", sf::Vector2f(SLIDERS_X_OFFSET, 550), pixel_font);

    auto unsolidify_button = init_momentary_button("Force", sf::Vector2f(SLIDERS_X_OFFSET + SLIDERS_WIDTH/2.0 + 5, 550), pixel_font);

    TileType element;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::F2) {
                    save_map_as_image(map, generate_filename());
                }
            }

            colorful_button.logic(window);
            brush_size_slider.logic(window);

            for (auto &brush_color_slider: brush_color_sliders) {
                brush_color_slider.logic(window);
            }

            if (colorful_button.get_state() && mouse_in_bounds(window_map_bounds, window) && left_mouse_button_pressed()) {
                for (size_t i = 0; i < brush_color_sliders.size() - 1; ++i) {
                    brush_color_sliders[i].set_slider_value((brush_color_sliders[i].get_slider_value() + 1) % 255);
                }
            }

            auto color = get_color_from_sliders(brush_color_sliders);

            update_sliders_colors(brush_color_sliders);

            solid_button.logic(window);
            handle_solid_button(solid_button.get_state(), element);

            color_preview_screen.setFillColor(color);

            if (left_mouse_button_pressed()) {
                use_brush(window, map, draw_map_offset, brush_size_slider.get_slider_value(), color, element);
            }
            if (right_mouse_button_pressed()) {
                use_brush(window, map, draw_map_offset, brush_size_slider.get_slider_value(), color, TileType::AIR);
            }

            reset_button.logic(window);
            handle_reset_button(reset_button.pressed(), map, brush_color_sliders);

            unsolidify_button.logic(window);
            handle_unsolidify_button(unsolidify_button.pressed(),map);

        }

        //Physics part
        apply_physics(map);

        //Drawing part
        window.clear();

        draw_map_as_sprite(window, draw_map_offset, map, map_size.x, map_size.y);
        brush_size_slider.draw(window);

        for (auto &brush_color_slider: brush_color_sliders) {
            brush_color_slider.draw(window);
        }
        unsolidify_button.draw(window);
        reset_button.draw(window);
        solid_button.draw(window);
        colorful_button.draw(window);
        window.draw(color_preview_screen);

        window.display();

    }


    return 0;
}

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <filesystem>
#include <cmath>

#include "components/Slider.h"
#include "utils/color_utils.h"

#define PIXEL_SIZE 2

struct tile{
    bool value;
    sf::Color color;
};

enum BRUSH_MODES{
    STANDARD,
    RUBBER
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

void use_brush(const sf::RenderWindow& window, map_t& map, sf::Vector2f offset, int radius, sf::Color color, enum BRUSH_MODES mode) {

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int grid_x = (mousePos.x - offset.x) / PIXEL_SIZE;
    int grid_y = (mousePos.y - offset.y) / PIXEL_SIZE;

    if (grid_x >= 0 && grid_x < map[0].size() && grid_y >= 0 && grid_y < map.size()) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int x = grid_x + dx;
                int y = grid_y + dy;
                if (x >= 0 && x < map[0].size() && y >= 0 && y < map.size()) {
                    if (mode == STANDARD) {
                        if (map[y][x].value != true){
                            if (dx * dx + dy * dy <= radius * radius) {
                                map[y][x].value = true;
                                map[y][x].color = color;
                            }
                        }
                    }
                    else if (mode == RUBBER) {
                        if (map[y][x].value == true){
                            if (dx * dx + dy * dy <= radius * radius) {
                                map[y][x].value = false;
                                map[y][x].color = color;
                            }
                        }
                    }

                }
            }
        }
    }
}

std::vector<Slider> init_color_sliders(int x, int y, int x_spcacing,int y_spacing, sf::Font& font) {

    Slider R_slider = Slider(sf::Vector2f(x,y),sf::Vector2f(100,25),0,255, font);
    R_slider.set_text("Red Value");
    R_slider.set_slider_color(sf::Color::Red);
    R_slider.set_slider_value(128);

    Slider G_slider = Slider(sf::Vector2f(x+x_spcacing,y+y_spacing),sf::Vector2f(100,25),0,255, font);
    G_slider.set_text("Green Value");
    G_slider.set_slider_color(sf::Color::Green);
    G_slider.set_slider_value(128);

    Slider B_slider = Slider(sf::Vector2f(x+2*x_spcacing,y+ 2*y_spacing ),sf::Vector2f(100,25),0,255, font);
    B_slider.set_text("Blue Value");
    B_slider.set_slider_color(sf::Color::Blue);
    B_slider.set_slider_value(128);

    std::vector brush_color_sliders = std::vector<Slider>();

    brush_color_sliders.push_back(R_slider);
    brush_color_sliders.push_back(G_slider);
    brush_color_sliders.push_back(B_slider);

    return brush_color_sliders;
}

int main() {

    sf::Vector2f window_size = sf::Vector2f(800,600);
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");

    const size_t width = 256;
    const size_t height = 256;

    map_t map(height, std::vector<tile>(width));

    auto draw_map_offset = sf::Vector2f(static_cast<unsigned int >(window_size.x/2 - (width*PIXEL_SIZE)/2), static_cast<unsigned int >(window_size.y/2 - (height*PIXEL_SIZE)/2));

    auto pixel_font = sf::Font();
    if (!pixel_font.openFromFile("assets/fonts/Pixel-Regular.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return EXIT_FAILURE;
    }


    Slider slider = Slider(sf::Vector2f(10,10),sf::Vector2f(100,25),0,20, pixel_font);
    slider.set_text("Brush Size");
    slider.set_slider_color(sf::Color(0,102,51));
    slider.set_slider_value(10);

    auto brush_color_sliders = init_color_sliders(10,200,0,50, pixel_font);


    sf::RectangleShape color_preview(sf::Vector2f(100,100));
    color_preview.setOutlineColor(sf::Color::White);
    auto start_color = sf::Color(brush_color_sliders[0].get_slider_value(), brush_color_sliders[1].get_slider_value(), brush_color_sliders[2].get_slider_value());
    color_preview.setFillColor(start_color);
    color_preview.setOutlineThickness(1);
    color_preview.setPosition(sf::Vector2f(10,75));


    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            slider.logic(window);

            for (auto& brush_color_slider : brush_color_sliders) {
                brush_color_slider.logic(window);
            }
            auto color = sf::Color(brush_color_sliders[0].get_slider_value(), brush_color_sliders[1].get_slider_value(), brush_color_sliders[2].get_slider_value());

            color_preview.setFillColor(color);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                use_brush(window, map,draw_map_offset, slider.get_slider_value(),color, STANDARD);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                use_brush(window, map,draw_map_offset, slider.get_slider_value(),color, RUBBER);
            }
        }

        window.clear();

        apply_physics(map);

        draw_map(window, draw_map_offset,map, width, height);

        slider.draw(window);

        for (auto& brush_color_slider : brush_color_sliders) {
            brush_color_slider.draw(window);
        }

        window.draw(color_preview);

        window.display();
    }

    return 0;
}

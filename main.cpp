#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <Windows.h>


#include "Tile/Tile.h"
#include "brush/brush.h"
#include "components/Button.h"
#include "components/Slider.h"
#include "sandmap/SandMap.h"


#define IMG_FOLDER "screenshots"

bool LEFT_MOUSE_BUTTON_STATE= false;

sf::RenderWindow init_screen(const sf::Vector2u window_size);
bool mouse_in_bounds(const sf::RectangleShape& bounds, const sf::RenderWindow &window);
sf::Font load_font();
sf::RectangleShape get_map_screen_bounds(const sf::Vector2u map_size, const sf::Vector2f screen_map_offset);
sf::RectangleShape init_preview_screen(sf::Vector2f position, sf::Vector2f size, sf::Color color);
Slider init_brush_size_slider(const sf::Vector2f position, const sf::Font &font);
sf::Color get_color_from_sliders(const std::vector<Slider>& brush_color_sliders);
bool left_mouse_button_pressed();
bool right_mouse_button_pressed();
std::string generate_filename();
sf::Vector2f calculate_map_offset(sf::Vector2u map_size, sf::Vector2f window_size);

int main() {
    std::filesystem::create_directories(IMG_FOLDER);

    auto window_size = sf::Vector2f(Config::SCREEN_WIDTH,Config::SCREEN_HEIGHT);
    auto map_size = sf::Vector2u(Config::MAP_WIDTH,Config::MAP_HEIGHT);

    sf::RenderWindow window = init_screen(sf::Vector2u(window_size));

    auto map = SandMap(map_size, window);

    auto draw_map_offset = calculate_map_offset(map_size, window_size);
    auto pixel_font = load_font();

    auto window_map_bounds = get_map_screen_bounds(
        map_size,
        draw_map_offset);

    auto brush_size_slider = init_brush_size_slider(sf::Vector2f(10,static_cast<float>(static_cast<int>(draw_map_offset.y))),pixel_font);

    auto brush_color_sliders = init_color_sliders(sf::Vector2f(10,250),0, 50,pixel_font,128);

    auto background_color_sliders = init_color_sliders(sf::Vector2f(10,600),0, 50,pixel_font, 0);

    auto start_color = sf::Color(brush_color_sliders[0].get_slider_value(), brush_color_sliders[1].get_slider_value(),
                                 brush_color_sliders[2].get_slider_value(), brush_color_sliders[3].get_slider_value());


    auto color_preview_screen = init_preview_screen(sf::Vector2f(10, 80),sf::Vector2f(Config::SLIDERS_WIDTH, Config::SLIDERS_WIDTH), start_color);

    auto reset_button = init_momentary_button("Clear", sf::Vector2f(10 + Config::SLIDERS_WIDTH/2.0 + 5, 450), pixel_font);

    auto solid_button = init_latching_button("Solid", sf::Vector2f(10, 450),sf::Vector2f(Config::SLIDERS_WIDTH/2.0 - 5,Config::SLIDERS_HEIGHT), pixel_font);

    auto R_color_button = init_latching_button("Rand", sf::Vector2f(10+ Config::SLIDERS_WIDTH + 10, 250),sf::Vector2f(Config::SLIDERS_HEIGHT,Config::SLIDERS_HEIGHT),pixel_font);

    auto G_color_button = init_latching_button("Rand", sf::Vector2f(10+ Config::SLIDERS_WIDTH + 10, 300),sf::Vector2f(Config::SLIDERS_HEIGHT,Config::SLIDERS_HEIGHT),pixel_font);

    auto B_color_button = init_latching_button("Rand", sf::Vector2f(10+ Config::SLIDERS_WIDTH + 10, 350),sf::Vector2f(Config::SLIDERS_HEIGHT,Config::SLIDERS_HEIGHT),pixel_font);

    auto A_color_button = init_latching_button("Rand", sf::Vector2f(10+ Config::SLIDERS_WIDTH + 10, 400),sf::Vector2f(Config::SLIDERS_HEIGHT,Config::SLIDERS_HEIGHT),pixel_font);

    auto unsolidify_button = init_momentary_button("Unsolidify", sf::Vector2f(10 + Config::SLIDERS_WIDTH/2.0 + 5, 500), pixel_font);

    auto solidify_button = init_momentary_button("Solidify", sf::Vector2f(10, 500), pixel_font);

    TileType element;

    auto color = get_color_from_sliders(brush_color_sliders);
    auto background_color = get_color_from_sliders(background_color_sliders);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::F2) {
                    map.save(generate_filename(), background_color);
                }
            }

            brush_size_slider.logic(window);

            for (auto &brush_color_slider: brush_color_sliders) {
                brush_color_slider.logic(window);
            }

            for (auto &background_color_slider: background_color_sliders) {
                background_color_slider.logic(window);
            }

            update_sliders_colors(brush_color_sliders);

            update_sliders_colors(background_color_sliders);

            solid_button.logic(window);
            handle_solid_button(solid_button.get_state(), element);

            reset_button.logic(window);
            handle_reset_button(reset_button.pressed(), map, brush_color_sliders);

            unsolidify_button.logic(window);
            handle_unsolidify_button(unsolidify_button.pressed(),map);

            solidify_button.logic(window);
            handle_solidify_button(solidify_button.pressed(),map);

            R_color_button.logic(window);
            G_color_button.logic(window);
            B_color_button.logic(window);
            A_color_button.logic(window);

            color_preview_screen.setFillColor(color);

        }

        if (R_color_button.get_state() && mouse_in_bounds(window_map_bounds, window) && left_mouse_button_pressed()) {
            brush_color_sliders[0].set_slider_value((brush_color_sliders[0].get_slider_value() + 1) % 255);
        }
        if (G_color_button.get_state() && mouse_in_bounds(window_map_bounds, window) && left_mouse_button_pressed()) {
            brush_color_sliders[1].set_slider_value((brush_color_sliders[1].get_slider_value() + 1) % 255);
        }
        if (B_color_button.get_state() && mouse_in_bounds(window_map_bounds, window) && left_mouse_button_pressed()) {
            brush_color_sliders[2].set_slider_value((brush_color_sliders[2].get_slider_value() + 1) % 255);
        }
        if (A_color_button.get_state() && mouse_in_bounds(window_map_bounds, window) && left_mouse_button_pressed()) {
            brush_color_sliders[3].set_slider_value((brush_color_sliders[3].get_slider_value() + 1) % 255);
        }

        color = get_color_from_sliders(brush_color_sliders);

        color_preview_screen.setFillColor(color);


        if (left_mouse_button_pressed()) {
            use_brush(window, map, draw_map_offset, brush_size_slider.get_slider_value(), color, element);
        }
        if (right_mouse_button_pressed()) {
            use_brush(window, map, draw_map_offset, brush_size_slider.get_slider_value(), color, TileType::AIR);
        }

        //Physics part
        map.applyPhysics();

        //Drawing part
        window.clear();
        background_color = get_color_from_sliders(background_color_sliders);

        map.draw(draw_map_offset,background_color);

        brush_size_slider.draw(window);

        for (auto &brush_color_slider: brush_color_sliders) {
            brush_color_slider.draw(window);
        }

        for (auto &background_color_slider: background_color_sliders) {
            background_color_slider.draw(window);
        }

        unsolidify_button.draw(window);
        reset_button.draw(window);
        solid_button.draw(window);
        solidify_button.draw(window);
        R_color_button.draw(window);
        G_color_button.draw(window);
        B_color_button.draw(window);
        A_color_button.draw(window);
        window.draw(color_preview_screen);

        window.display();

    }

    return 0;
}

sf::RenderWindow init_screen(const sf::Vector2u window_size) {
    sf::VideoMode video_mode(window_size);
    auto style = sf::Style::Titlebar | sf::Style::Close;
    sf::RenderWindow window(video_mode, "SandSim", style);

    sf::Image icon;
    if (icon.loadFromFile("assets/icon/sandSim.png")) {
        window.setIcon(icon);
    } else {
        std::cerr << "Cannot load icon form: " << std::endl;
    }

    return window;
}
bool mouse_in_bounds(const sf::RectangleShape& bounds, const sf::RenderWindow &window) {
    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    return bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
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
    auto map_rect = sf::RectangleShape(sf::Vector2f(static_cast<float>(map_size.x * Config::PIXEL_SIZE), static_cast<float>(map_size.y * Config::PIXEL_SIZE)));
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

    auto slider = Slider(position, sf::Vector2f(Config::SLIDERS_WIDTH,Config::SLIDERS_HEIGHT), 0, 64, font);
    slider.set_text("Brush Size");
    slider.set_slider_color(sf::Color(0, 102, 51));
    slider.set_slider_value(10);

    return slider;
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
std::string generate_filename() {

    const std::time_t now = std::time(nullptr);
    const std::tm *tm_ptr = std::localtime(&now);

    std::ostringstream ss;
    ss << IMG_FOLDER << "/screen_"
            << std::put_time(tm_ptr, "%Y-%m-%d_%H-%M-%S")
            << ".png";

    return ss.str();
}
sf::Vector2f calculate_map_offset(sf::Vector2u map_size, sf::Vector2f window_size) {
    return sf::Vector2f(static_cast<float>((window_size.x/2.0 - (map_size.x * Config::PIXEL_SIZE)/2.0)),static_cast<float>((window_size.y/2.0 - (map_size.y * Config::PIXEL_SIZE)/2.0)));
}

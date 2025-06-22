//
// Created by anton on 05.05.2025.
//

#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <Config.h>

class Slider{
private:
    sf::RectangleShape slider_frame;
    sf::RectangleShape slider;
    int slider_value;
    int max_slider_value;
    int min_slider_value;

    sf::Text slider_label;
    float SLIDER_VALUE_LABEL_Y_OFFSET = 0.5f;
    sf::Text slider_value_label;

    bool is_mouse_inside(sf::Vector2i mousePos) const;

public:
    Slider(sf::Vector2f position, sf::Vector2f size, int min_value, int max_value, const sf::Font& font);

    void set_slider_color(sf::Color new_color);

    void draw(sf::RenderWindow& window);

    void logic(sf::RenderWindow& window);

    int get_slider_value() const;

    void set_text(const std::string& text);

    void set_slider_value(int value);

};

std::vector<Slider> init_color_sliders(const sf::Vector2f position, const int x_spacing, const int y_spacing, const sf::Font &font, const uint8_t sliders_start_value);
void update_sliders_colors(std::vector<Slider>& brush_color_sliders);

#endif //SLIDER_H

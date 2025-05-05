//
// Created by anton on 05.05.2025.
//

#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <string>

class Slider {
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
    Slider(const sf::Vector2f position, const sf::Vector2f size, int min_value, int max_value, const sf::Font& font);

    void set_slider_color(sf::Color new_color);

    void draw(sf::RenderWindow& window);

    void logic(sf::RenderWindow& window);

    int get_slider_value() const;

    void set_text(const std::string& text);

    void set_slider_value(int value);

};


#endif //SLIDER_H

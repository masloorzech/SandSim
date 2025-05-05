//
// Created by anton on 05.05.2025.
//
#include "Slider.h"


bool Slider::is_mouse_inside(sf::Vector2i mousePos) const {
return (mousePos.x > slider_frame.getPosition().x &&
        mousePos.x < slider_frame.getPosition().x + slider_frame.getSize().x &&
        mousePos.y > slider_frame.getPosition().y &&
        mousePos.y < slider_frame.getPosition().y + slider_frame.getSize().y);
}

Slider::Slider(const sf::Vector2f position, const sf::Vector2f size, int min_value, int max_value, const sf::Font& font):
slider_label(font), slider_value_label(font) {
    slider_frame.setSize(size);
    slider_frame.setPosition(sf::Vector2f(position.x, position.y));
    slider_frame.setFillColor(sf::Color::Black);
    slider_frame.setOutlineColor(sf::Color::White);
    slider_frame.setOutlineThickness(1);


    this->min_slider_value = min_value;
    this->max_slider_value = max_value;
    this->slider_value = 0;

    slider.setSize(sf::Vector2f(0, size.y));

    slider.setPosition(sf::Vector2f(position.x, position.y));

    slider.setFillColor(sf::Color::Red);

    slider_label = sf::Text(font, "", 8);
    slider_label.setFillColor(sf::Color::White);
    slider_label.setPosition(sf::Vector2f(position.x, position.y - slider_frame.getSize().y / 4));

    slider_value_label = sf::Text(font, std::to_string(this->slider_value+1), 8);
    slider_value_label.setFillColor(sf::Color::White);
    slider_value_label.setPosition(sf::Vector2f(position.x, position.y + SLIDER_VALUE_LABEL_Y_OFFSET * size.y));

}


void Slider::set_slider_color(sf::Color new_color) {
    slider.setFillColor(new_color);
}


void Slider:: draw(sf::RenderWindow& window) {
    window.draw(slider_frame);

    float percent = static_cast<float>(slider_value - min_slider_value) / (max_slider_value - min_slider_value);
    float slider_width = percent * slider_frame.getSize().x;

    slider.setSize(sf::Vector2f(slider_width, slider_frame.getSize().y));

    window.draw(slider);

    window.draw(this->slider_label);

    window.draw(this->slider_value_label);
}

void Slider::logic(sf::RenderWindow& window){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (is_mouse_inside(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        float rel_x = mousePos.x - slider_frame.getPosition().x;
        float percent = rel_x / slider_frame.getSize().x;
        percent = std::clamp(percent, 0.0f, 1.0f);
        slider_value = static_cast<int>(min_slider_value + percent * (max_slider_value - min_slider_value));
        slider_value_label.setString(std::to_string(slider_value+1));
    }
}

int Slider::get_slider_value() const {
    return slider_value;
}

void Slider::set_text(const std::string& text) {
    this->slider_label.setString(text);
}

void Slider::set_slider_value(int value) {
    if (value < min_slider_value) {
        this->slider_value = min_slider_value;
    }
    else if (value > max_slider_value) {
        this->slider_value = max_slider_value;
    }
    else {
        this->slider_value = value;
    }
}

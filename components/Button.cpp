//
// Created by anton on 06.05.2025.
//

#include "Button.h"

#include <cmath>

Button::Button(const sf::Vector2f position, const sf::Vector2f size, const sf::Color color, const sf::Font& font):
label(font){
    button.setPosition(position);
    button.setSize(size);

    button_nonpressed_color = color;

    button.setFillColor(button_nonpressed_color);
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(1);

    label.setFont(font);
    label.setCharacterSize(8);
    label.setFillColor(sf::Color::White);

    dirty= true;
}

void Button::draw(sf::RenderWindow& window){
  if (dirty){
      window.draw(button);
      window.draw(label);
  }

}

void Button::set_button_color(const sf::Color new_color) {
    button.setFillColor(new_color);
    dirty = true;
}

void Button::set_new_button_pressed_color(const sf::Color new_color) {
    button_pressed_color = new_color;
}

void Button::set_new_button_hover_color(const sf::Color new_color) {
    button_hover_color = new_color;
}

void Button::set_new_button_nonpressed_color(const sf::Color new_color) {
    button_nonpressed_color = new_color;
}


bool Button::mouse_in_bounds(const sf::RenderWindow& window) const {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos));

}

void Button::set_text(const std::string& text) {
    label.setString(text);

    sf::FloatRect bounds = label.getLocalBounds();

    label.setOrigin(sf::Vector2f(
        std::round(bounds.position.x + bounds.size.x / 2.f),
        std::round(bounds.position.y + bounds.size.y / 2.f)));

    label.setPosition(sf::Vector2f(
        std::round(button.getPosition().x + (button.getSize().x / 2.f)),
        std::round(button.getPosition().y + (button.getSize().y / 2.f))));

    dirty = true;
}

void MomentaryButton::logic(sf::RenderWindow& window) {
    if (mouse_in_bounds(window)) {
        set_button_color(button_hover_color);
        dirty = true;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            last_pressed_count = press_count;
            press_count++;
            set_button_color(button_pressed_color);
        }
    } else {
        set_button_color(button_nonpressed_color);
    }
}
bool MomentaryButton::pressed(){
    if (press_count > last_pressed_count) {
        press_count=0;
        last_pressed_count = 0;
        return true;
    }
    return false;
}


void LatchingButton::logic(sf::RenderWindow& window) {
    if (mouse_in_bounds(window)) {
        dirty = true;
        set_button_color(button_hover_color);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (!wasPressed) {
                wasPressed = true;
                latched = !latched;
            }
        } else {
            wasPressed = false;
        }
    } else {
        wasPressed = false;
        set_button_color(latched ? button_pressed_color : button_nonpressed_color);
    }
}

bool LatchingButton::get_state() const {
    return latched;
}

void LatchingButton::change_state(bool state) {
    latched = state;
}

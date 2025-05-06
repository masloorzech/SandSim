//
// Created by anton on 06.05.2025.
//

#include "Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Font& font):
label(font){
    button.setPosition(position);
    button.setSize(size);
    button.setFillColor(color);

    button_nonpressed_color = color;

    button.setFillColor(button_nonpressed_color);
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(1);

    label.setFont(font);
    label.setCharacterSize(18);

    label.setPosition(sf::Vector2f(position.x + 10.f, position.y + (size.y / 2.f - label.getCharacterSize() / 2.f)));

    dirty= true;
}

void Button::draw(sf::RenderWindow& window){
  if (dirty){
      window.draw(button);

      window.draw(label);
  }

}

void Button::set_button_color(sf::Color new_color) {
    button.setFillColor(new_color);
    dirty = true;
}

void Button::set_new_button_pressed_color(sf::Color new_color) {
    button_pressed_color = new_color;
}

void Button::set_new_button_hover_color(sf::Color new_color) {
    button_hover_color = new_color;
}

void Button::set_new_button_nonpressed_color(sf::Color new_color) {
    button_nonpressed_color = new_color;
}


bool Button::mouse_in_bounds(sf::RenderWindow& window) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos));

}

void Button::set_text(const std::string& text) {
    label.setString(text);
    dirty = true;
}

void MomentaryButton::logic(sf::RenderWindow& window) {

    if (mouse_in_bounds(window)) {
        set_button_color(button_hover_color);
        dirty = true;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            set_button_color(button_pressed_color);
        }
    } else {
        set_button_color(button_nonpressed_color);
        dirty = false;
    }
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
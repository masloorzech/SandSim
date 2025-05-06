//
// Created by anton on 06.05.2025.
//

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H


#include <SFML/Graphics.hpp>
#include <functional>


class Button{
    protected:
        sf::RectangleShape button;
        sf::Color button_pressed_color;
        sf::Color button_nonpressed_color;
        sf::Color button_hover_color;

        sf::Text label;

        bool dirty;

        void set_button_color(sf::Color new_color);

    public:

      Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, const sf::Font& font);
      virtual ~Button() = default;

      void draw(sf::RenderWindow &window);

      void set_new_button_nonpressed_color(sf::Color new_color);

      void set_new_button_pressed_color(sf::Color new_color);

      void set_new_button_hover_color(sf::Color new_color);

      bool mouse_in_bounds(const sf::RenderWindow &window) const;

      void set_text(const std::string& text);

      virtual void logic(sf::RenderWindow &window) = 0;
};

class MomentaryButton : public Button{
    long press_count;
    long last_pressed_count;
    public:
      using Button::Button;
      void logic(sf::RenderWindow &window) override;
    bool pressed();
};

class LatchingButton : public Button{
    bool wasPressed = false;

    bool latched = false;

  public:

    using Button::Button;
    void logic(sf::RenderWindow &window) override;
    bool get_state() const;
    void change_state(bool state);
};


#endif //SELECTION_MENU_H

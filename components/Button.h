//
// Created by anton on 06.05.2025.
//

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H


#include <SFML/Graphics.hpp>

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

      Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Font& font);
      virtual ~Button() = default;

      void draw(sf::RenderWindow &window);

      void set_new_button_nonpressed_color(sf::Color new_color);

      void set_new_button_pressed_color(sf::Color new_color);

      void set_new_button_hover_color(sf::Color new_color);

      bool mouse_in_bounds(sf::RenderWindow &window);

      void set_text(const std::string& text);

      virtual void logic(sf::RenderWindow &window) = 0;
};

class MomentaryButton : public Button{
    public:
      using Button::Button;
      void logic(sf::RenderWindow &window);
};

class LatchingButton : public Button{
  private:
    bool wasPressed = false;

    bool latched = false;
  public:
    using Button::Button;
    void logic(sf::RenderWindow &window);
};


#endif //SELECTION_MENU_H

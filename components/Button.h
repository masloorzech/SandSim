//
// Created by anton on 06.05.2025.
//

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H


#include <SFML/Graphics.hpp>
#include <functional>
#include <Slider.h>
#include <sandmap/SandMap.h>

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

void handle_reset_button(const bool state, SandMap& map, std::vector<Slider>& brush_color_sliders);
void handle_solid_button(const bool state, TileType& element);
void handle_unsolidify_button(const bool state, SandMap& map);
void handle_solidify_button(const bool state, SandMap& map);
LatchingButton init_latching_button(const std::string& text, const sf::Vector2f position, const sf::Vector2f size ,const sf::Font &font);
MomentaryButton init_momentary_button(const std::string &text, const sf::Vector2f position, const sf::Font &font);
#endif //SELECTION_MENU_H

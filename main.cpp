#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <filesystem>
#include <cmath>

#define PIXEL_SIZE 2

#define SAND_COLOR sf::Color(242,210,169)

struct tile{
    bool value;
    sf::Color color;
};

int color_incremeter = 0;

sf::Color HSLtoRGB(float h, float s, float l) {
    float c = (1 - std::fabs(2 * l - 1)) * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
    float m = l - c / 2;

    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    return sf::Color((r + m) * 255, (g + m) * 255, (b + m) * 255);
}

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

void place_sand(const sf::RenderWindow& window, map_t& map, sf::Vector2f offset, float time, int radius) {

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int grid_x = (mousePos.x - offset.x) / PIXEL_SIZE;
    int grid_y = (mousePos.y - offset.y) / PIXEL_SIZE;

    if (grid_x >= 0 && grid_x < map[0].size() && grid_y >= 0 && grid_y < map.size()) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int x = grid_x + dx;
                int y = grid_y + dy;
                if (x >= 0 && x < map[0].size() && y >= 0 && y < map.size()) {
                    if (map[y][x].value != true){
                        if (dx * dx + dy * dy <= radius * radius) {
                            map[y][x].value = true;
                            map[y][x].color = HSLtoRGB(std::fmod(time + (x + y) * 20.0f, 360.0f), 0.6f, 0.5f);
                        }
                    }
                }
            }
        }
    }
}


class Slider {
    sf::RectangleShape slider_frame;
    sf::RectangleShape slider;
    int slider_value;
    int max_slider_value;
    int min_slider_value;

    sf::Text slider_label;
    float SLIDER_VALUE_LABEL_Y_OFFSET = 0.5;
    sf::Text slider_value_label;

    bool is_mouse_inside(sf::Vector2i mousePos) const {
        return (mousePos.x > slider_frame.getPosition().x &&
                mousePos.x < slider_frame.getPosition().x + slider_frame.getSize().x &&
                mousePos.y > slider_frame.getPosition().y &&
                mousePos.y < slider_frame.getPosition().y + slider_frame.getSize().y);
    }


    public:

    Slider(const sf::Vector2f position, const sf::Vector2f size, int min_value, int max_value, const sf::Font& font):
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

        slider.setFillColor(SAND_COLOR);

        slider_label = sf::Text(font, "", 8);
        slider_label.setFillColor(sf::Color::White);
        slider_label.setPosition(sf::Vector2f(position.x, position.y - slider_frame.getSize().y / 4));

        slider_value_label = sf::Text(font, std::to_string(this->slider_value+1), 8);
        slider_value_label.setFillColor(sf::Color::White);
        slider_value_label.setPosition(sf::Vector2f(position.x, position.y + SLIDER_VALUE_LABEL_Y_OFFSET * size.y));

    }


    void set_slider_color(sf::Color new_color) {
        slider.setFillColor(new_color);
    }


    void draw(sf::RenderWindow& window) {
        window.draw(slider_frame);

        float percent = static_cast<float>(slider_value - min_slider_value) / (max_slider_value - min_slider_value);
        float slider_width = percent * slider_frame.getSize().x;

        slider.setSize(sf::Vector2f(slider_width, slider_frame.getSize().y));

        window.draw(slider);

        window.draw(this->slider_label);

        window.draw(this->slider_value_label);
    }

    void logic(sf::RenderWindow& window){
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (is_mouse_inside(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            float rel_x = mousePos.x - slider_frame.getPosition().x;
            float percent = rel_x / slider_frame.getSize().x;
            percent = std::clamp(percent, 0.0f, 1.0f);
            slider_value = static_cast<int>(min_slider_value + percent * (max_slider_value - min_slider_value));
            slider_value_label.setString(std::to_string(slider_value+1));
        }
    }

    int get_slider_value() const {
        return slider_value;
    }

    void set_text(const std::string& text) {
        this->slider_label.setString(text);
    }

};



int main() {

    sf::Vector2f window_size = sf::Vector2f(800,600);
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_size.x), static_cast<unsigned int>(window_size.y)}), "SandSim");

    const size_t width = 256;
    const size_t height = 256;

    map_t map(height, std::vector<tile>(width));

    auto draw_map_offset = sf::Vector2f(static_cast<unsigned int >(window_size.x/2 - (width*PIXEL_SIZE)/2), static_cast<unsigned int >(window_size.y/2 - (height*PIXEL_SIZE)/2));

    float time = 0.0f;

    auto pixel_font = sf::Font();
    if (!pixel_font.openFromFile("assets/fonts/Pixel-Regular.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return EXIT_FAILURE;
    }


    Slider slider = Slider(sf::Vector2f(10,10),sf::Vector2f(100,25),0,20, pixel_font);
    slider.set_text("Brush Size");


    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                place_sand(window, map,draw_map_offset, time, slider.get_slider_value());
                }
            slider.logic(window);
            }



        window.clear();

        apply_physics(map);

        draw_map(window, draw_map_offset,map, width, height);

        slider.draw(window);

        window.display();
        time += 0.001f;
        if (time >= 360.0f) time = 0.0f;
    }

    return 0;
}

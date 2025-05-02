#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>


int main(){
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800,600}), "SandSim");

    int width =20;
    int height =20;
    int pixel_size = 20;
    bool map[width][height];

    sf::RectangleShape tile(sf::Vector2f(pixel_size, pixel_size));
    tile.setFillColor(sf::Color::Black);
    tile.setOutlineThickness(1);
    tile.setOutlineColor(sf::Color::White);

    while (window.isOpen()){

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        for (int i=0; i<height; i++){
            for (int j =0; j<width; j++){
                tile.setPosition(sf::Vector2f(i*pixel_size,j*pixel_size));
                window.draw(tile);
            }
        }

        window.display();
    }

    return 0;

}
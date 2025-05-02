#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#define PIXEL_SIZE 25

void draw_map(sf::RenderWindow window,bool** map, size_t width, size_t height){

    sf::RectangleShape tile(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
    tile.setFillColor(sf::Color::Black);
    tile.setOutlineThickness(1);
    tile.setOutlineColor(sf::Color::White);

    for (size_t i=0; i<height; i++){
        for (size_t j =0; j<width; j++){
            if (*(*(map + i) + j)){
                tile.setFillColor(sf::Color::White);
                window.draw(tile);
                tile.setFillColor(sf::Color::Black);
            }
            else {
                tile.setPosition(sf::Vector2f(i * PIXEL_SIZE, j * PIXEL_SIZE));
                window.draw(tile);
            }
        }
    }

}

int main(){
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800,600}), "SandSim");

    int width =20;
    int height =20;
    bool map[width][height];



    while (window.isOpen()){

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();


        window.display();
    }

    return 0;

}
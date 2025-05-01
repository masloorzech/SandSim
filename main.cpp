#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>


int main(){
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800,600}), "SandSim");

    while (window.isOpen()){

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
        window.display();
    }

    return 0;

}
#include <SFML/Graphics.hpp>
#include "Mechanism.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Mechanism");


    
    mech::MechanicSystemFabric system(400,400);
    mech::JointPoint jointPoint(400, 400);


    system.push_mech(std::make_unique<mech::Crank>(10, 23, &jointPoint));
    system.push_mech(std::make_unique<mech::Rod>(100,50, &jointPoint));

    
    bool isMousePressed = false;
    sf::Vector2i lastMousePos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isMousePressed = true;
                    lastMousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isMousePressed = false;
                }
            }
        }

        if (isMousePressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            jointPoint.update(mousePos);
        }

        system.updateParts();

        window.clear(sf::Color::White);
        system.drawParts(window);
        window.display();
    }

    return 0;
}

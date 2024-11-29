#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Utils.h"

void drawArrow(sf::RenderWindow& window, float startX, float startY, float endX, float endY) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(startX, startY), sf::Color::Yellow),
        sf::Vertex(sf::Vector2f(endX, endY), sf::Color::Yellow)
    };
    window.draw(line, 2, sf::Lines);

    // Cabeza de la flecha
    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, sf::Vector2f(endX, endY));
    arrowHead.setPoint(1, sf::Vector2f(endX - 10, endY - 5));
    arrowHead.setPoint(2, sf::Vector2f(endX - 10, endY + 5));
    arrowHead.setFillColor(sf::Color::Yellow);
    window.draw(arrowHead);
}

void runLinkedListVisualization(sf::RenderWindow& window) {
    std::vector<int> elements;
    bool isInsertMode = true;  // Bandera para alternar entre modos
    std::string inputBuffer;
    sf::Event event;

    const int nodeRadius = 30;
    const int nodeSpacing = 100;
    const int baseX = 100;
    const int baseY = 300;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    inputBuffer += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\b' && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                } else if (event.text.unicode == 13) {  // Enter
                    if (inputBuffer.empty()) {
                        isInsertMode = !isInsertMode;  // Cambiar modo
                    } else {
                        if (isInsertMode) {
                            elements.push_back(std::stoi(inputBuffer));
                        } else {
                            int pos = std::stoi(inputBuffer);
                            if (pos >= 0 && pos < elements.size()) {
                                elements.erase(elements.begin() + pos);
                            }
                        }
                        inputBuffer.clear();
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::LShift) {
                    return;  // Regresar al menú principal
                }
            }
        }

        window.clear(sf::Color::Black);

        // Dibujar nodos de la lista enlazada
        for (size_t i = 0; i < elements.size(); ++i) {
            float x = baseX + i * nodeSpacing;
            float y = baseY;

            // Dibujar círculo del nodo
            sf::CircleShape node(nodeRadius);
            node.setFillColor(sf::Color::Green);
            node.setOutlineColor(sf::Color::White);
            node.setOutlineThickness(2);
            node.setPosition(x - nodeRadius, y - nodeRadius);
            window.draw(node);

            // Dibujar valor dentro del nodo
            sf::Text valueText(std::to_string(elements[i]), font, 20);
            valueText.setFillColor(sf::Color::White);
            valueText.setPosition(x - nodeRadius / 2, y - nodeRadius / 2);
            window.draw(valueText);

            // Dibujar flecha al siguiente nodo
            if (i < elements.size() - 1) {
                drawArrow(window, x + nodeRadius, y, x + nodeSpacing - nodeRadius, y);
            }
        }

        // Dibujar "NULL" después del último nodo
        if (!elements.empty()) {
            float x = baseX + elements.size() * nodeSpacing;
            float y = baseY;

            drawArrow(window, x - nodeSpacing / 2, y, x, y);

            sf::Text nullText("NULL", font, 20);
            nullText.setFillColor(sf::Color::White);
            nullText.setPosition(x + 10, y - 10);
            window.draw(nullText);
        }

        // Dibujar título centrado
        sf::Text title("Linked Lists", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar modo actual
        sf::Text modeText(isInsertMode ? "Modo actual: Insertar" : "Modo actual: Eliminar", font, 25);
        modeText.setStyle(sf::Text::Bold);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 60);
        window.draw(modeText);

        // Mostrar instrucciones y entrada
        sf::Text instructions("", font, 15);
        instructions.setFillColor(sf::Color::White);
        instructions.setPosition(10, 120);
        if (isInsertMode) {
            instructions.setString("Ingrese un numero para insertar y presione Enter.");
        } else {
            instructions.setString("Ingrese la posicion para eliminar el nodo y presione Enter.");
        }
        window.draw(instructions);

        sf::Text inputText(inputBuffer, font, 15);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(10, 150);
        window.draw(inputText);

        // Mostrar mensaje para regresar al menú
        sf::Text menuMessage("Presione Esc o Shift para regresar al menu anterior.", font, 15);
        menuMessage.setFillColor(sf::Color::White);
        menuMessage.setPosition(10, 180);
        window.draw(menuMessage);

        window.display();
    }
}

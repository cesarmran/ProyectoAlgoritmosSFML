#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Stacks.h"
#include "Utils.h"

void runStackVisualization(sf::RenderWindow& window) {
    std::vector<int> elements;
    bool isPushMode = true;  // Bandera para alternar entre modos
    std::string inputBuffer;
    sf::Event event;

    const int width = 100;
    const int height = 30;
    const int baseX = 400;  // Coordenada X de la base de la pila
    const int baseY = 500;  // Coordenada Y de la base de la pila

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
                        // Si el usuario presiona Enter sin introducir número, cambia de modo
                        isPushMode = !isPushMode;
                    } else {
                        if (isPushMode) {
                            // Apilar (Push)
                            elements.push_back(std::stoi(inputBuffer));
                        }
                        inputBuffer.clear();
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && !isPushMode && !elements.empty()) {
                    // Desapilar (Pop) en modo eliminar
                    elements.pop_back();
                } else if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::LShift) {
                    // Regresar al menú principal
                    return;
                }
            }
        }

        window.clear(sf::Color::Black);

        // Dibujar bloques de la pila
        for (size_t i = 0; i < elements.size(); ++i) {
            sf::RectangleShape block(sf::Vector2f(width, height));

            // Colorear bloques:
            if (i == elements.size() - 1) {
                // Último bloque: cyan con borde amarillo
                block.setFillColor(sf::Color::Cyan);
                block.setOutlineColor(sf::Color::Yellow);
            } else {
                // Resto de los bloques: rojo con borde blanco
                block.setFillColor(sf::Color::Red);
                block.setOutlineColor(sf::Color::White);
            }
            block.setOutlineThickness(2);
            block.setPosition(baseX, baseY - i * height);
            window.draw(block);

            // Dibujar texto del elemento en el bloque
            sf::Text text(std::to_string(elements[i]), font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(baseX + width / 4, baseY - i * height + 5);
            window.draw(text);
        }

        // Dibujar título centrado
        sf::Text title("Pilas (Stacks)", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar modo actual
        sf::Text modeText(isPushMode ? "Modo actual: Apilar (Push)" : "Modo actual: Eliminar (Pop)", font, 25);
        modeText.setStyle(sf::Text::Bold);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 60);
        window.draw(modeText);

        // Dibujar instrucciones y mensajes adicionales
        sf::Text instructions("", font, 15);
        instructions.setFillColor(sf::Color::White);
        if (isPushMode) {
            instructions.setString("Presione Enter para cambiar al modo eliminar.");
        } else {
            instructions.setString("Presione Enter para cambiar al modo apilar.\nPresione Espacio para desapilar el bloque superior.");
        }
        instructions.setPosition(10, 120);
        window.draw(instructions);

        // Mostrar mensaje para regresar al menú principal
        sf::Text menuMessage("Presione Esc o Shift para regresar al menu anterior.", font, 15);
        menuMessage.setFillColor(sf::Color::White);
        menuMessage.setPosition(10, 200);
        window.draw(menuMessage);

        // Mostrar el número que se está ingresando
        if (isPushMode) {
            sf::Text inputText("Ingrese un numero para apilar:", font, 15);
            inputText.setFillColor(sf::Color::White);
            inputText.setPosition(10, 240);
            window.draw(inputText);

            sf::Text numberText(inputBuffer, font, 15);
            numberText.setFillColor(sf::Color::White);
            numberText.setPosition(10, 270);
            window.draw(numberText);
        }

        window.display();
    }
}

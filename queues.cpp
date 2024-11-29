#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Utils.h"

void runQueueVisualization(sf::RenderWindow& window) {
    std::vector<int> elements;
    bool isEnqueueMode = true;  // Bandera para alternar entre modos
    std::string inputBuffer;
    sf::Event event;

    const int width = 100;
    const int height = 30;
    const int baseX = 100;  // Coordenada X de la base de la cola (más a la izquierda)
    const int baseY = 500;  // Coordenada Y de la base de la cola

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
                        isEnqueueMode = !isEnqueueMode;
                    } else {
                        if (isEnqueueMode) {
                            // Encolar (Enqueue)
                            elements.push_back(std::stoi(inputBuffer));
                        }
                        inputBuffer.clear();
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && !isEnqueueMode && !elements.empty()) {
                    // Desencolar (Dequeue) en modo eliminar
                    elements.erase(elements.begin());  // Elimina el primer elemento
                } else if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::LShift) {
                    // Regresar al menú principal
                    return;
                }
            }
        }

        window.clear(sf::Color::Black);

        // Dibujar bloques de la cola
        for (size_t i = 0; i < elements.size(); ++i) {
            sf::RectangleShape block(sf::Vector2f(width, height));

            // Colorear bloques:
            if (i == 0) {
                // Primer bloque: cyan con borde amarillo
                block.setFillColor(sf::Color::Cyan);
                block.setOutlineColor(sf::Color::Yellow);
            } else {
                // Resto de los bloques: rojo con borde blanco
                block.setFillColor(sf::Color::Red);
                block.setOutlineColor(sf::Color::White);
            }
            block.setOutlineThickness(2);
            block.setPosition(baseX + i * (width + 10), baseY);  // Espaciado horizontal
            window.draw(block);

            // Dibujar texto del elemento en el bloque
            sf::Text text(std::to_string(elements[i]), font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(baseX + i * (width + 10) + width / 4, baseY + 5);
            window.draw(text);
        }

        // Dibujar título centrado
        sf::Text title("Colas (Queues)", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar modo actual
        sf::Text modeText(isEnqueueMode ? "Modo actual: Encolar (Enqueue)" : "Modo actual: Eliminar (Dequeue)", font, 25);
        modeText.setStyle(sf::Text::Bold);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 60);
        window.draw(modeText);

        // Dibujar instrucciones y mensajes adicionales
        sf::Text instructions("", font, 15);
        instructions.setFillColor(sf::Color::White);
        if (isEnqueueMode) {
            instructions.setString("Presione Enter para cambiar al modo eliminar.");
        } else {
            instructions.setString("Presione Enter para cambiar al modo encolar.\nPresione Espacio para desencolar el bloque frontal.");
        }
        instructions.setPosition(10, 120);
        window.draw(instructions);

        // Mostrar mensaje para regresar al menú principal
        sf::Text menuMessage("Presione Esc o Shift para regresar al menu anterior.", font, 15);
        menuMessage.setFillColor(sf::Color::White);
        menuMessage.setPosition(10, 200);
        window.draw(menuMessage);

        // Mostrar el número que se está ingresando
        if (isEnqueueMode) {
            sf::Text inputText("Ingrese un numero para encolar:", font, 15);
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

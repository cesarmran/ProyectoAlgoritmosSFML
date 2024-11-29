#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include "utils.h"

void runLinearSearch(sf::RenderWindow& window) {
    int numElements = 0;
    std::vector<int> vec;
    int target = -1;
    bool isInputtingNumElements = true;
    bool isInputtingVector = false;
    bool isInputtingTarget = false;
    bool isSearching = false;
    bool isFinished = false;
    bool found = false;
    int currentIndex = -1;
    int foundIndex = -1;

    sf::Clock clock;
    sf::Event event;
    std::string inputBuffer;
    int currentInputIndex = 0;
    std::chrono::steady_clock::time_point searchStart;
    std::chrono::steady_clock::time_point searchEnd;
    double durationInSeconds = 0.0;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Clock animationClock; // Controla la velocidad de la animación

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Verificar si se presiona la tecla Shift (izquierda o derecha)
            if (event.type == sf::Event::KeyPressed &&
                (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift)) {
                return; // Regresar al menú principal
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    inputBuffer += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\b' && !inputBuffer.empty()) { // Backspace
                    inputBuffer.pop_back();
                } else if (event.text.unicode == 13) { // Enter
                    if (isInputtingNumElements) {
                        numElements = std::stoi(inputBuffer);
                        vec.resize(numElements, 0);
                        inputBuffer.clear();
                        isInputtingNumElements = false;
                        isInputtingVector = true;
                    } else if (isInputtingVector) {
                        vec[currentInputIndex++] = std::stoi(inputBuffer);
                        inputBuffer.clear();
                        if (currentInputIndex >= numElements) {
                            isInputtingVector = false;
                            isInputtingTarget = true;
                        }
                    } else if (isInputtingTarget) {
                        target = std::stoi(inputBuffer);
                        inputBuffer.clear();
                        isInputtingTarget = false;

                        // Comienza la medición del tiempo real del algoritmo
                        searchStart = std::chrono::steady_clock::now();

                        // Realizar búsqueda para medir tiempo real del algoritmo
                        for (size_t i = 0; i < vec.size(); ++i) {
                            if (vec[i] == target) {
                                found = true;
                                foundIndex = static_cast<int>(i);
                                break;
                            }
                        }

                        searchEnd = std::chrono::steady_clock::now();
                        durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - searchStart).count() / 1000.0;

                        // Iniciar animación para visualización
                        currentIndex = -1;
                        isSearching = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Centrar título en la ventana
        sf::Text title("Linear Search", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar animación de búsqueda
        if (!isInputtingNumElements && !isInputtingVector && !isInputtingTarget) {
            float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
            float barHeight = 50; // Altura incrementada
            for (size_t i = 0; i < vec.size(); ++i) {
                sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
                bar.setPosition(i * barWidth + 10, window.getSize().y - 150); // Barra más grande

                if (static_cast<int>(i) == currentIndex) {
                    bar.setFillColor(sf::Color::Yellow);
                } else if (static_cast<int>(i) == foundIndex) {
                    bar.setFillColor(sf::Color::Green); // Indicar el valor encontrado
                } else {
                    bar.setFillColor(sf::Color::White);
                }

                window.draw(bar);

                // Mostrar índice dentro de las barras
                sf::Text index(std::to_string(i), font, 15);
                index.setFillColor(sf::Color::Black); // Color del índice
                index.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y + 30);
                window.draw(index);

                // Mostrar valores arriba de las barras
                sf::Text value(std::to_string(vec[i]), font, 15);
                value.setFillColor(sf::Color::White); // Color del texto
                value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20); // Colocar encima de la barra
                window.draw(value);
            }
        }

        // Lógica de animación para visualización, más lenta
        if (isSearching && animationClock.getElapsedTime().asSeconds() >= 0.5f) {
            animationClock.restart(); // Reinicia el temporizador
            if (currentIndex < static_cast<int>(vec.size()) - 1) {
                currentIndex++;
                if (vec[currentIndex] == target) {
                    isSearching = false;
                }
            }
            if (currentIndex >= static_cast<int>(vec.size()) - 1) {
                isSearching = false;
            }
        }

        // Mostrar texto de entrada y resultados
        sf::Text text("", font, 20);
        text.setFillColor(sf::Color::White);
        if (isInputtingNumElements) {
            text.setString("Numero de elementos del array:\n" + inputBuffer);
            text.setPosition(10, 100);
        } else if (isInputtingVector) {
            text.setString("Introduzca el elemento [" + std::to_string(currentInputIndex + 1) + "]:\n" + inputBuffer);
            text.setPosition(10, 100);
        } else if (isInputtingTarget) {
            std::ostringstream ss;
            ss << "Vector introducido: [";
            for (size_t i = 0; i < vec.size(); ++i) {
                ss << vec[i];
                if (i != vec.size() - 1) ss << ", ";
            }
            ss << "]\nIntroduce el numero a buscar:\n" + inputBuffer;
            text.setString(ss.str());
            text.setPosition(10, 100);
        } else if (!isSearching) {
            std::ostringstream ss;
            ss << "Vector introducido: [";
            for (size_t i = 0; i < vec.size(); ++i) {
                ss << vec[i];
                if (i != vec.size() - 1) ss << ", ";
            }
            ss << "]\nElemento a buscar: " + std::to_string(target) + "\n";
            if (found) {
                ss << "El elemento " + std::to_string(target) + " fue encontrado en el indice " + std::to_string(foundIndex) + "\n";
            } else {
                ss << "El elemento no fue encontrado.\n";
            }
            ss << "Tiempo de ejecucion del algoritmo: " + std::to_string(durationInSeconds) + " s\nPresione SHIFT para regresar al menu.";
            text.setString(ss.str());
            text.setPosition(10, 100);
        }
        window.draw(text);

        window.display();
    }
}

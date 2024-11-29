#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm> // Para std::sort
#include "utils.h"

void runBinarySearch(sf::RenderWindow& window) {
    int numElements = 0;
    std::vector<int> vec;
    int target = -1;
    bool isInputtingNumElements = true;
    bool isInputtingVector = false;
    bool isInputtingTarget = false;
    bool isSearching = false;
    bool isFinished = false;
    bool found = false;
    int left = 0, right = 0, mid = 0;
    int foundIndex = -1;

    sf::Event event;
    std::string inputBuffer;
    int currentInputIndex = 0;
    std::chrono::steady_clock::time_point searchStart;
    std::chrono::steady_clock::time_point searchEnd;
    double durationInSeconds = 0.0; // Tiempo del algoritmo

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Clock animationClock; // Controla la velocidad de la animación

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
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
                        isSearching = true;

                        // Iniciar la medición del tiempo del algoritmo
                        searchStart = std::chrono::steady_clock::now();

                        // Ordenar el vector para Binary Search
                        std::sort(vec.begin(), vec.end());
                        left = 0;
                        right = vec.size() - 1;

                        // Ejecutar Binary Search para calcular el tiempo real del algoritmo
                        while (left <= right) {
                            mid = left + (right - left) / 2;

                            if (vec[mid] == target) {
                                found = true;
                                foundIndex = mid;
                                break;
                            } else if (vec[mid] < target) {
                                left = mid + 1;
                            } else {
                                right = mid - 1;
                            }
                        }

                        // Finalizar la medición del tiempo del algoritmo
                        searchEnd = std::chrono::steady_clock::now();
                        durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - searchStart).count() / 1000.0;

                        // Reiniciar el rango para la animación
                        left = 0;
                        right = vec.size() - 1;
                    } else if (isFinished) {
                        return; // Regresar al menú principal
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Centrar título en la ventana
        sf::Text title("Binary Search", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar animación de búsqueda
        if (!isInputtingNumElements && !isInputtingVector && !isInputtingTarget) {
            float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
            float barHeight = 50; // Altura de las barras
            for (size_t i = 0; i < vec.size(); ++i) {
                sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
                bar.setPosition(i * barWidth + 10, window.getSize().y - 150);

                // Colorear barras según el estado
                if (i < left || i > right) {
                    bar.setFillColor(sf::Color::Red); // Fuera del rango actual
                } else if (i == static_cast<size_t>(mid)) {
                    bar.setFillColor(sf::Color::Yellow); // Índice actual
                } else {
                    bar.setFillColor(sf::Color::White); // Barras dentro del rango
                }

                window.draw(bar);

                // Mostrar valores arriba de las barras
                sf::Text value(std::to_string(vec[i]), font, 15);
                value.setFillColor(sf::Color::White);
                value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
                window.draw(value);

                // Mostrar índices dentro de las barras
                sf::Text index(std::to_string(i), font, 15);
                index.setFillColor(sf::Color::Black);
                index.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y + 10);
                window.draw(index);
            }
        }

        // Lógica de animación para Binary Search
        if (isSearching && animationClock.getElapsedTime().asSeconds() >= 0.8f) {
            animationClock.restart();
            if (left <= right) {
                mid = left + (right - left) / 2;

                if (vec[mid] == target) {
                    isSearching = false;
                } else if (vec[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            } else {
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
            ss << "Tiempo de ejecucion del algoritmo: " + std::to_string(durationInSeconds) + " s\nPresione SHIFT1 para regresar al menu.";
            text.setString(ss.str());
            text.setPosition(10, 100);
        }
        window.draw(text);

        window.display();
    }
}

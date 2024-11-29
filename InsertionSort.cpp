#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include "Utils.h"

// Implementación del algoritmo Insertion Sort con animación
void runInsertionSort(sf::RenderWindow& window, const std::string& algorithmName) {
    int numElements = 0;
    std::vector<int> vec;
    bool isInputtingNumElements = true;
    bool isInputtingVector = false;
    bool shouldExit = false;
    int currentInputIndex = 0;
    sf::Clock animationClock;
    std::chrono::steady_clock::time_point start, end;
    double durationInSeconds = 0.0;

    sf::Event event;
    std::string inputBuffer;

    // Variables para la animación
    std::vector<int> originalVec;
    bool isAnimating = false;
    bool animationFinished = false;
    int currentIndex = 1;  // Comenzamos en el segundo elemento
    int movingIndex = 0;
    int key = 0;

    while (window.isOpen() && !shouldExit) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    inputBuffer += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\b' && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                } else if (event.text.unicode == 13) { // Enter
                    if (isInputtingNumElements) {
                        numElements = std::stoi(inputBuffer);
                        vec.resize(numElements);
                        inputBuffer.clear();
                        isInputtingNumElements = false;
                        isInputtingVector = true;
                    } else if (isInputtingVector) {
                        vec[currentInputIndex++] = std::stoi(inputBuffer);
                        inputBuffer.clear();
                        if (currentInputIndex >= numElements) {
                            originalVec = vec;
                            isInputtingVector = false;

                            // Calcular tiempo del algoritmo
                            start = std::chrono::steady_clock::now();
                            std::vector<int> tempVec = vec; // Simulación para medir tiempo
                            for (size_t i = 1; i < tempVec.size(); ++i) {
                                int tempKey = tempVec[i];
                                int tempJ = i - 1;
                                while (tempJ >= 0 && tempVec[tempJ] > tempKey) {
                                    tempVec[tempJ + 1] = tempVec[tempJ];
                                    --tempJ;
                                }
                                tempVec[tempJ + 1] = tempKey;
                            }
                            end = std::chrono::steady_clock::now();
                            durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

                            // Preparar animación
                            isAnimating = true;
                            currentIndex = 1;
                            key = vec[currentIndex];  // Inicializar con el segundo elemento
                            movingIndex = currentIndex;
                        }
                    } else if (isAnimating || animationFinished) {
                        shouldExit = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        // Centrar el título
        sf::Text title(algorithmName, font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar barras para la animación
        if (isAnimating || animationFinished) {
            float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
            float maxBarHeight = 200.0f;
            int maxValue = !originalVec.empty() ? *std::max_element(originalVec.begin(), originalVec.end()) : 1;
            for (size_t i = 0; i < vec.size(); ++i) {
                float barHeight = maxBarHeight * vec[i] / maxValue;
                sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
                bar.setPosition(i * barWidth + 10, window.getSize().y - barHeight - 50);

                // Resaltar barras según su estado
                if (isAnimating && i == currentIndex) {
                    bar.setFillColor(sf::Color::Yellow); // Barra en comparación
                } else if (isAnimating && i == movingIndex) {
                    bar.setFillColor(sf::Color::Blue); // Barra que se está moviendo
                } else if (i < currentIndex || animationFinished) {
                    bar.setFillColor(sf::Color::Green); // Barras ordenadas
                } else {
                    bar.setFillColor(sf::Color::White); // Barras sin ordenar
                }
                window.draw(bar);

                // Dibujar valores encima de las barras
                sf::Text value(std::to_string(vec[i]), font, 15);
                value.setFillColor(sf::Color::White);
                value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
                window.draw(value);
            }

            // Lógica de animación para Insertion Sort
            if (isAnimating && animationClock.getElapsedTime().asSeconds() >= 0.5f) {
                animationClock.restart();
                if (movingIndex > 0 && vec[movingIndex - 1] > key) {
                    vec[movingIndex] = vec[movingIndex - 1];
                    --movingIndex;
                } else {
                    vec[movingIndex] = key; // Colocar el valor correcto
                    ++currentIndex;
                    if (currentIndex < static_cast<int>(vec.size())) {
                        key = vec[currentIndex];
                        movingIndex = currentIndex;
                    } else {
                        isAnimating = false; // Terminar animación
                        animationFinished = true;
                    }
                }
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
        } else if (!isAnimating) {
            std::ostringstream ssOriginal, ssSorted;
            ssOriginal << "Vector original: [";
            for (size_t i = 0; i < originalVec.size(); ++i) {
                ssOriginal << originalVec[i];
                if (i != originalVec.size() - 1) ssOriginal << ", ";
            }
            ssOriginal << "]";
            ssSorted << "Vector ordenado: [";
            for (size_t i = 0; i < vec.size(); ++i) {
                ssSorted << vec[i];
                if (i != vec.size() - 1) ssSorted << ", ";
            }
            ssSorted << "]";
            text.setString(ssOriginal.str() + "\n" + ssSorted.str() + "\nTiempo de ejecucion del algoritmo: " +
                           std::to_string(durationInSeconds) + " segundos\nPresione 'Enter' para regresar.");
            text.setPosition(10, 100);
        }
        window.draw(text);

        window.display();
    }
}

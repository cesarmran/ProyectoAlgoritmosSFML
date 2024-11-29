#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include "BubbleSort.h"  // Asegúrate de incluir el encabezado adecuado para la función bubbleSort
#include "Utils.h"

void bubbleSort(std::vector<int>& vec) {
    bool swapped;
    int n = vec.size();
    do {
        swapped = false;
        for (int i = 1; i < n; ++i) {
            if (vec[i - 1] > vec[i]) {
                std::swap(vec[i - 1], vec[i]);
                swapped = true;
            }
        }
        --n; // Reducir el rango en cada pasada
    } while (swapped);
}
void runSortingAlgorithm(sf::RenderWindow& window, const std::string& algorithmName) {
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

    // Copia del vector original para mostrar resultados
    std::vector<int> originalVec;

    // Variables para la animación de Bubble Sort
    bool isAnimating = false;
    int currentIndex = 0;
    int passIndex = 0;
    bool swapped = false;

    while (window.isOpen() && !shouldExit) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
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
                            // Calcular tiempo de ejecución del algoritmo
                            start = std::chrono::steady_clock::now();
                            bubbleSort(vec); // Ejecutar Bubble Sort
                            end = std::chrono::steady_clock::now();
                            durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
                            // Iniciar animación
                            vec = originalVec; // Restaurar vector original para animación
                            isAnimating = true;
                            passIndex = vec.size();
                        }
                    } else {
                        shouldExit = true; // Salir al menú principal
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Centrar el título
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text title(algorithmName, font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar barras para la animación
        if (!isInputtingNumElements && !isInputtingVector && isAnimating) {
            float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
            float maxBarHeight = 200.0f;
            int maxValue = *std::max_element(originalVec.begin(), originalVec.end());
            for (size_t i = 0; i < vec.size(); ++i) {
                float barHeight = maxBarHeight * vec[i] / maxValue;
                sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
                bar.setPosition(i * barWidth + 10, window.getSize().y - barHeight - 50);

                // Resaltar las barras en comparación
                if (isAnimating && (i == currentIndex || i == currentIndex - 1)) {
                    bar.setFillColor(sf::Color::Yellow);
                } else if (i >= passIndex) {
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

            // Animación paso a paso del algoritmo Bubble Sort
            if (animationClock.getElapsedTime().asSeconds() >= 0.5f) {
                animationClock.restart();
                if (currentIndex < passIndex - 1) {
                    if (vec[currentIndex] > vec[currentIndex + 1]) {
                        std::swap(vec[currentIndex], vec[currentIndex + 1]);
                        swapped = true;
                    }
                    currentIndex++;
                } else {
                    if (!swapped) {
                        // Finalizar animación
                        isAnimating = false;
                    }
                    passIndex--;
                    currentIndex = 0;
                    swapped = false;
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

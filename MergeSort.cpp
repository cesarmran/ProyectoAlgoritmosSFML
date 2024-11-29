#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm> // Para std::max_element
#include "Utils.h"

// Función Merge para animación
void merge(std::vector<int>& vec, int left, int mid, int right, std::vector<int>& animVec) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArray(n1), rightArray(n2);

    for (int i = 0; i < n1; ++i)
        leftArray[i] = vec[left + i];
    for (int i = 0; i < n2; ++i)
        rightArray[i] = vec[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            animVec[k] = leftArray[i];
            vec[k] = leftArray[i];
            ++i;
        } else {
            animVec[k] = rightArray[j];
            vec[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        animVec[k] = leftArray[i];
        vec[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        animVec[k] = rightArray[j];
        vec[k] = rightArray[j];
        ++j;
        ++k;
    }
}

// Función Merge Sort sin animación (para medir tiempo de ejecución del algoritmo)
void mergeSort(std::vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        std::vector<int> dummyAnim(vec.size());
        merge(vec, left, mid, right, dummyAnim);
    }
}

// Función Merge Sort con animación
void mergeSortAnim(std::vector<int>& vec, int left, int right, std::vector<int>& animVec, sf::RenderWindow& window, sf::Font& font) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortAnim(vec, left, mid, animVec, window, font);
        mergeSortAnim(vec, mid + 1, right, animVec, window, font);

        // Mostrar animación
        merge(vec, left, mid, right, animVec);

        // Dibujar animación paso a paso
        window.clear(sf::Color::Black);
        float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
        float maxBarHeight = 200.0f;
        int maxValue = *std::max_element(vec.begin(), vec.end());

        for (size_t i = 0; i < animVec.size(); ++i) {
            float barHeight = maxBarHeight * animVec[i] / maxValue;
            sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
            bar.setPosition(i * barWidth + 10, window.getSize().y - barHeight - 250);
            bar.setFillColor(sf::Color::Yellow);
            window.draw(bar);

            sf::Text value(std::to_string(animVec[i]), font, 15);
            value.setFillColor(sf::Color::White);
            value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
            window.draw(value);
        }
        window.display();

        sf::Clock pauseClock;
        while (pauseClock.getElapsedTime().asSeconds() < 0.5f) {}
    }
}

// Función principal de ejecución para Merge Sort
void runMergeSort(sf::RenderWindow& window, const std::string& algorithmName) {
    int numElements = 0;
    std::vector<int> vec;
    bool isInputtingNumElements = true;
    bool isInputtingVector = false;
    bool shouldExit = false;
    int currentInputIndex = 0;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Event event;
    std::string inputBuffer;

    // Variables para la animación
    std::vector<int> originalVec;
    std::vector<int> animVec;

    // Tiempo de ejecución del algoritmo
    double durationInSeconds = 0.0;

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
                        animVec.resize(numElements);
                        inputBuffer.clear();
                        isInputtingNumElements = false;
                        isInputtingVector = true;
                    } else if (isInputtingVector) {
                        vec[currentInputIndex++] = std::stoi(inputBuffer);
                        inputBuffer.clear();
                        if (currentInputIndex >= numElements) {
                            originalVec = vec;
                            animVec = vec;
                            isInputtingVector = false;

                            // Calcular tiempo del algoritmo
                            auto start = std::chrono::steady_clock::now();
                            mergeSort(vec, 0, vec.size() - 1); // Algoritmo puro
                            auto end = std::chrono::steady_clock::now();
                            durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

                            // Ejecutar animación
                            mergeSortAnim(vec, 0, vec.size() - 1, animVec, window, font);
                        }
                    } else {
                        shouldExit = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Centrar el título
        sf::Text title(algorithmName, font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Mostrar texto informativo
        if (!isInputtingNumElements && !isInputtingVector) {
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
            sf::Text text(ssOriginal.str() + "\n" + ssSorted.str() +
                          "\nTiempo de ejecucion del algoritmo: " +
                          std::to_string(durationInSeconds) + " segundos\nPresione 'Enter' para regresar.",
                          font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(10, 150);
            window.draw(text);
        }

        // Mostrar las barras ordenadas al final de la animación
        if (!isInputtingNumElements && !isInputtingVector) {
            float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
            float maxBarHeight = 200.0f;
            int maxValue = *std::max_element(originalVec.begin(), originalVec.end());
            for (size_t i = 0; i < vec.size(); ++i) {
                float barHeight = maxBarHeight * vec[i] / maxValue;
                sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
                bar.setPosition(i * barWidth + 10, window.getSize().y - barHeight - 50);
                bar.setFillColor(sf::Color::Green);
                window.draw(bar);

                sf::Text value(std::to_string(vec[i]), font, 15);
                value.setFillColor(sf::Color::White);
                value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
                window.draw(value);
            }
        }

        // Mostrar valores y solicitud de entrada
        sf::Text text("", font, 20);
        if (isInputtingNumElements) {
            text.setString("Numero de elementos del array:\n" + inputBuffer);
            text.setPosition(10, 100);
            window.draw(text);
        } else if (isInputtingVector) {
            text.setString("Introduzca el elemento [" + std::to_string(currentInputIndex + 1) + "]:\n" + inputBuffer);
            text.setPosition(10, 100);
            window.draw(text);
        }

        window.display();
    }
}

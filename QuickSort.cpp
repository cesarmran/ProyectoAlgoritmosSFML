#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm>
#include "Utils.h"

// Implementación de QuickSort básica
int partition(std::vector<int>& vec, int low, int high) {
    int pivot = vec[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (vec[j] <= pivot) {
            ++i;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quickSort(std::vector<int>& vec, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(vec, low, high);
        quickSort(vec, low, pivotIndex - 1);
        quickSort(vec, pivotIndex + 1, high);
    }
}

// Partición con animación
int partitionAnim(std::vector<int>& vec, int low, int high, sf::RenderWindow& window, sf::Font& font) {
    int pivot = vec[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (vec[j] <= pivot) {
            ++i;
            std::swap(vec[i], vec[j]);
        }

        // Animación del proceso
        window.clear(sf::Color::Black);

        float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
        float maxBarHeight = 200.0f;
        int maxValue = *std::max_element(vec.begin(), vec.end());

        for (size_t k = 0; k < vec.size(); ++k) {
            float barHeight = maxBarHeight * vec[k] / maxValue;
            sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
            bar.setPosition(k * barWidth + 10, window.getSize().y - barHeight - 200);

            if (k == high) {
                bar.setFillColor(sf::Color::Red); // Pivote
            } else if (k == i || k == j) {
                bar.setFillColor(sf::Color::Yellow); // Índices en comparación
            } else {
                bar.setFillColor(sf::Color::White); // Barras normales
            }
            window.draw(bar);

            sf::Text value(std::to_string(vec[k]), font, 15);
            value.setFillColor(sf::Color::White);
            value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
            window.draw(value);
        }

        window.display();
        sf::Clock pauseClock;
        while (pauseClock.getElapsedTime().asSeconds() < 0.5f) {}
    }

    std::swap(vec[i + 1], vec[high]);

    // Mostrar barras después de mover el pivote
    window.clear(sf::Color::Black);
    float barWidth = static_cast<float>(window.getSize().x) / (vec.size() + 1);
    float maxBarHeight = 200.0f;
    int maxValue = *std::max_element(vec.begin(), vec.end());
    for (size_t k = 0; k < vec.size(); ++k) {
        float barHeight = maxBarHeight * vec[k] / maxValue;
        sf::RectangleShape bar(sf::Vector2f(barWidth - 5, barHeight));
        bar.setPosition(k * barWidth + 10, window.getSize().y - barHeight - 200);
        bar.setFillColor(k == i + 1 ? sf::Color::Green : sf::Color::White);
        window.draw(bar);

        sf::Text value(std::to_string(vec[k]), font, 15);
        value.setFillColor(sf::Color::White);
        value.setPosition(bar.getPosition().x + barWidth / 4, bar.getPosition().y - 20);
        window.draw(value);
    }
    window.display();
    sf::Clock pauseClock;
    while (pauseClock.getElapsedTime().asSeconds() < 0.5f) {}

    return i + 1;
}

void quickSortAnim(std::vector<int>& vec, int low, int high, sf::RenderWindow& window, sf::Font& font) {
    if (low < high) {
        int pivotIndex = partitionAnim(vec, low, high, window, font);
        quickSortAnim(vec, low, pivotIndex - 1, window, font);
        quickSortAnim(vec, pivotIndex + 1, high, window, font);
    }
}

void runQuickSort(sf::RenderWindow& window, const std::string& algorithmName) {
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

    std::vector<int> originalVec;

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
                } else if (event.text.unicode == 13) {
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

                            auto start = std::chrono::steady_clock::now();
                            quickSort(vec, 0, vec.size() - 1);
                            auto end = std::chrono::steady_clock::now();
                            durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

                            quickSortAnim(vec, 0, vec.size() - 1, window, font);
                        }
                    } else {
                        shouldExit = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        sf::Text title(algorithmName, font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

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

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include "LinearSearch.h"
#include "BinarySearch.h"
#include "BubbleSort.h"
#include "QuickSort.h"
#include "SelectionSort.h"
#include "MergeSort.h"
#include "InsertionSort.h"
#include "LinkedLists.h"
#include "queues.h"
#include "stacks.h"
#include "AVL.h"
#include "Dijkstra.h"
#include "DFS.h"
#include "utils.h"

using namespace std;

// Funciones de dibujo
void drawMenu(sf::RenderWindow& window, int highlightedOption);
void handleOption(int option, sf::RenderWindow& window);
void handleSearchOption(int option, bool& backToMain, sf::RenderWindow& window) {
    switch (option) {
        case 0: // Linear Search
            try {
                runLinearSearch(window);
            } catch (const std::exception& e) {
                std::cerr << "Error en Linear Search: " << e.what() << std::endl;
            }
        break;
        case 1: // Binary Search
            try {
                runBinarySearch(window);
            } catch (const std::exception& e) {
                std::cerr << "Error en Binary Search: " << e.what() << std::endl;
            }
        break;
        case 2: // Regresar al menú principal
            backToMain = true;
        break;
        default:
            std::cerr << "Opción inválida: " << option << std::endl;
        break;
    }
}


void drawSearchAnimation(sf::RenderWindow& window) {
    // Crear dos barras para simular una animación
    sf::RectangleShape bar1(sf::Vector2f(100, 30));
    bar1.setFillColor(sf::Color::Red);
    bar1.setPosition(200, 400); // Más abajo

    sf::RectangleShape bar2(sf::Vector2f(100, 30));
    bar2.setFillColor(sf::Color::Green);
    bar2.setPosition(400, 450); // Más abajo y separadas

    // Movimiento básico (puedes modificar para hacerlo más dinámico)
    static float offset = 0; // Control del desplazamiento
    offset += 2.0f; // Mayor velocidad
    if (offset > 600) offset = 0; // Reiniciar al salir de la ventana

    bar1.setPosition(200 + offset, 400);
    bar2.setPosition(400 - offset, 450);

    // Dibujar las barras en la ventana
    window.draw(bar1);
    window.draw(bar2);
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu de Algoritmos", sf::Style::Close | sf::Style::Resize);
    void drawSearchAnimation(sf::RenderWindow& window);
    int selectedOption = -1;
    int highlightedOption = 0; // Controla cuál opción está resaltada

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5) {
                    selectedOption = event.key.code - sf::Keyboard::Num1 + 1;
                    handleOption(selectedOption, window);
                } else if (event.key.code == sf::Keyboard::Up) {
                    highlightedOption = (highlightedOption - 1 + 5) % 5; // Mover hacia arriba
                } else if (event.key.code == sf::Keyboard::Down) {
                    highlightedOption = (highlightedOption + 1) % 5; // Mover hacia abajo
                } else if (event.key.code == sf::Keyboard::Enter) {
                    handleOption(highlightedOption + 1, window); // Selección con Enter
                }
            }
        }

        window.clear();
        drawMenu(window, highlightedOption); // Dibuja el menú con la opción resaltada
        drawSearchAnimation(window); // Llama a la animación
        window.display();
    }

    return 0;
}

void drawMenu(sf::RenderWindow& window, int highlightedOption) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error al cargar la fuente" << endl;
        return;
    }

    // Título del menú
    sf::Text title("Menu de Algoritmos", font, 30);
    title.setStyle(sf::Text::Bold); // Aplicar estilo en negrita
    title.setPosition(250, 50);
    title.setFillColor(sf::Color::White);

    vector<string> options = {
        "1. Algoritmos de Busqueda",
        "2. Algoritmos de Ordenamiento",
        "3. Estructuras Lineales",
        "4. Estructuras No Lineales",
        "5. Salir"
    };

    // Dibuja las opciones del menú
    window.draw(title);
    for (int i = 0; i < options.size(); ++i) {
        sf::Text option(options[i], font, 20);
        option.setPosition(100, 150 + i * 50);

        // Resaltar la opción seleccionada
        if (i == highlightedOption) {
            option.setFillColor(sf::Color::Yellow);
        } else {
            option.setFillColor(sf::Color::White);
        }
        drawSearchAnimation(window);
        window.draw(option);
    }
}

void handleOption(int option, sf::RenderWindow& window) {

    switch (option) {
      case 1: {
    // Submenu de Algoritmos de Búsqueda
    bool backToMain = false;
    int highlightedOption = 0; // Opción resaltada inicialmente

    while (!backToMain) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                // Manejo de teclas numéricas para ejecutar directamente
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num3) {
                    int selectedOption = event.key.code - sf::Keyboard::Num1;
                    handleSearchOption(selectedOption, backToMain, window);
                }
                // Manejo de las flechas para mover el resaltado
                else if (event.key.code == sf::Keyboard::Up) {
                    highlightedOption = (highlightedOption - 1 + 3) % 3; // Navegar hacia arriba
                } else if (event.key.code == sf::Keyboard::Down) {
                    highlightedOption = (highlightedOption + 1) % 3; // Navegar hacia abajo
                }
                // Manejo de la tecla Enter para ejecutar la opción resaltada
                else if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Opción resaltada seleccionada: " << highlightedOption << std::endl;
                    handleSearchOption(highlightedOption, backToMain, window);
                }
            }
        }

        window.clear(); // Limpiar la pantalla
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error al cargar la fuente" << std::endl;
            return;
        }

        // Título del submenú
        sf::Text title("Algoritmos de Busqueda", font, 30);
        title.setStyle(sf::Text::Bold);
        title.setPosition(200, 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);

        // Opciones del submenú
        vector<string> searchOptions = {
            "1. Linear Search",
            "2. Binary Search",
            "3. Regresar al Menu Principal"
        };

        // Dibujar las opciones con resaltado dinámico
        for (int i = 0; i < searchOptions.size(); ++i) {
            sf::Text option(searchOptions[i], font, 20);
            option.setPosition(100, 150 + i * 50);
            if (i == highlightedOption) {
                option.setFillColor(sf::Color::Yellow); // Resaltar la opción activa
            } else {
                option.setFillColor(sf::Color::White);
            }
            window.draw(option);
        }

        // Llamar a la animación
        drawSearchAnimation(window);

        window.display(); // Mostrar la ventana actualizada
    }
    break;
}



        case 2: {
            // Submenu de Algoritmos de Ordenamiento
            bool backToMain = false;
            int highlightedSubOption = 0; // Control de resaltado inicial
            while (!backToMain) {
                 sf::Event event;
                 while (window.pollEvent(event)) {
                     if (event.type == sf::Event::Closed) {
                        window.close();
                         return;
            }

            if (event.type == sf::Event::KeyPressed) {
                // Manejo de teclas numéricas para ejecutar directamente
                if (event.key.code == sf::Keyboard::Num1) {
                    runSortingAlgorithm(window, "Bubble Sort");
                } else if (event.key.code == sf::Keyboard::Num2) {
                    runSelectionSort(window, "Selection Sort");
                } else if (event.key.code == sf::Keyboard::Num3) {
                    runQuickSort(window, "Quick Sort");
                } else if (event.key.code == sf::Keyboard::Num4) {
                    runMergeSort(window, "Merge Sort");
                } else if (event.key.code == sf::Keyboard::Num5) {
                    runInsertionSort(window, "Insertion Sort");
                } else if (event.key.code == sf::Keyboard::Num6) {
                    backToMain = true; // Regresar al menú principal
                }

                // Manejo de las flechas para mover el resaltado
                if (event.key.code == sf::Keyboard::Up) {
                    highlightedSubOption = (highlightedSubOption - 1 + 6) % 6; // Navegar hacia arriba
                } else if (event.key.code == sf::Keyboard::Down) {
                    highlightedSubOption = (highlightedSubOption + 1) % 6; // Navegar hacia abajo
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Ejecutar según la opción resaltada
                    if (highlightedSubOption == 0) runSortingAlgorithm(window, "Bubble Sort");
                    else if (highlightedSubOption == 1) runSelectionSort(window, "Selection Sort");
                    else if (highlightedSubOption == 2) runQuickSort(window, "Quick Sort");
                    else if (highlightedSubOption == 3) runMergeSort(window, "Merge Sort");
                    else if (highlightedSubOption == 4) runInsertionSort(window, "Insertion Sort");
                    else if (highlightedSubOption == 5) backToMain = true; // Regresar al menú principal
                }
            }
        }

        window.clear(); // Limpiar la pantalla
        sf::Font font;
        font.loadFromFile("arial.ttf");

        // Título del submenú
        sf::Text title("Algoritmos de Ordenamiento", font, 30);
                title.setStyle(sf::Text::Bold);
        title.setPosition(200, 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);

        // Opciones del submenú
        vector<string> sortingOptions = {
            "1. Bubble Sort",
            "2. Selection Sort",
            "3. Quick Sort",
            "4. Merge Sort",
            "5. Insertion Sort",
            "6. Regresar al Menu Principal"
        };

        // Dibujar opciones con resaltado dinámico
        for (int i = 0; i < sortingOptions.size(); ++i) {
            sf::Text option(sortingOptions[i], font, 20);
            option.setPosition(100, 150 + i * 50);
            if (i == highlightedSubOption) {
                option.setFillColor(sf::Color::Yellow); // Resaltar la opción
            } else {
                option.setFillColor(sf::Color::White);
            }
            window.draw(option);
        }

        window.display(); // Mostrar la ventana actualizada
    }
    break;
}

        case 3: {
    bool backToMain = false;
    int highlightedSubOption = 0; // Control de resaltado inicial
    while (!backToMain) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                // Manejo de teclas numéricas para ejecutar directamente
                if (event.key.code == sf::Keyboard::Num1) {
                    runStackVisualization(window);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    runQueueVisualization(window);
                } else if (event.key.code == sf::Keyboard::Num3) {
                    runLinkedListVisualization(window);
                } else if (event.key.code == sf::Keyboard::Num4) {
                    backToMain = true; // Regresar al menú principal
                }

                // Manejo de las flechas para mover el resaltado
                if (event.key.code == sf::Keyboard::Up) {
                    highlightedSubOption = (highlightedSubOption - 1 + 4) % 4; // Navegar hacia arriba
                } else if (event.key.code == sf::Keyboard::Down) {
                    highlightedSubOption = (highlightedSubOption + 1) % 4; // Navegar hacia abajo
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Ejecutar según la opción resaltada
                    if (highlightedSubOption == 0) runStackVisualization(window);
                    else if (highlightedSubOption == 1) runQueueVisualization(window);
                    else if (highlightedSubOption == 2) runLinkedListVisualization(window);
                    else if (highlightedSubOption == 3) backToMain = true; // Regresar al menú principal
                }
            }
        }

        window.clear(); // Limpiar la pantalla
        sf::Font font;
        font.loadFromFile("arial.ttf");

        // Título del submenú
        sf::Text title("Estructuras Lineales", font, 30);
        title.setStyle(sf::Text::Bold);
        title.setPosition(200, 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);

        // Opciones del submenú
        vector<string> options = {
            "1. Filas (Stacks)",
            "2. Colas (Queues)",
            "3. Listas Enlazadas (Linked Lists)",
            "4. Regresar al Menu Principal"
        };

        // Dibujar opciones con resaltado dinámico
        for (int i = 0; i < options.size(); ++i) {
            sf::Text option(options[i], font, 20);
            option.setPosition(100, 150 + i * 50);
            if (i == highlightedSubOption) {
                option.setFillColor(sf::Color::Yellow); // Resaltar la opción
            } else {
                option.setFillColor(sf::Color::White);
            }
            window.draw(option);
        }

        window.display(); // Mostrar la ventana actualizada
    }
    break;
}

        case 4: {
    bool backToMain = false;
    int highlightedSubOption = 0; // Control de resaltado inicial
    while (!backToMain) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                // Manejo de teclas numéricas para ejecutar directamente
                if (event.key.code == sf::Keyboard::Num1) {
                    runAVL(window);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    runDijkstra(window);
                } else if (event.key.code == sf::Keyboard::Num3) {
                    runGraphSearch(window);
                } else if (event.key.code == sf::Keyboard::Num4) {
                    backToMain = true; // Regresar al menú principal
                }

                // Manejo de las flechas para mover el resaltado
                if (event.key.code == sf::Keyboard::Up) {
                    highlightedSubOption = (highlightedSubOption - 1 + 4) % 4; // Navegar hacia arriba
                } else if (event.key.code == sf::Keyboard::Down) {
                    highlightedSubOption = (highlightedSubOption + 1) % 4; // Navegar hacia abajo
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Ejecutar según la opción resaltada
                    if (highlightedSubOption == 0) runAVL(window);
                    else if (highlightedSubOption == 1) runDijkstra(window);
                    else if (highlightedSubOption == 2) runGraphSearch(window);
                    else if (highlightedSubOption == 3) backToMain = true; // Regresar al menú principal
                }
            }
        }

        window.clear(); // Limpiar la pantalla
        sf::Font font;
        font.loadFromFile("arial.ttf");

        // Título del submenú
        sf::Text title("Estructuras No Lineales", font, 30);
        title.setStyle(sf::Text::Bold);
        title.setPosition(200, 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);

        // Opciones del submenú
        vector<string> options = {
            "1. Arboles AVL",
            "2. Dijkstra",
            "3. Grafos",
            "4. Regresar al Menu Principal"
        };

        // Dibujar opciones con resaltado dinámico
        for (int i = 0; i < options.size(); ++i) {
            sf::Text option(options[i], font, 20);
            option.setPosition(100, 150 + i * 50);
            if (i == highlightedSubOption) {
                option.setFillColor(sf::Color::Yellow); // Resaltar la opción
            } else {
                option.setFillColor(sf::Color::White);
            }
            window.draw(option);
        }

        window.display(); // Mostrar la ventana actualizada
    }
    break;
}

        case 5:
            window.close();  // Salir del programa
        break;
        default:
            break;
    }
}


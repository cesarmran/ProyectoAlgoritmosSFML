#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <stdexcept>

// Estructura para las aristas del grafo
struct Edge {
    int target;
};

// Función para dibujar texto
void drawTexto(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size, const sf::Font& font, sf::Color color = sf::Color::White) {
    sf::Text sfText(text, font, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(color);
    sfText.setStyle(sf::Text::Bold);
    window.draw(sfText);
}

// Función para dibujar nodos
void drawNode(sf::RenderWindow& window, const sf::Vector2f& position, const std::string& label, const sf::Font& font, sf::Color color) {
    sf::CircleShape circle(20);
    circle.setFillColor(color);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setPosition(position - sf::Vector2f(20, 20));
    window.draw(circle);

    sf::Text text(label, font, 20);
    text.setPosition(position - sf::Vector2f(10, 10));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Función para dibujar aristas
void drawEdge(sf::RenderWindow& window, const sf::Vector2f& from, const sf::Vector2f& to) {
    sf::Vertex line[] = {
        sf::Vertex(from, sf::Color::White),
        sf::Vertex(to, sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);
}

// Función para verificar existencia en un unordered_map
bool nodeExists(const std::unordered_map<int, sf::Vector2f>& positions, int node) {
    return positions.find(node) != positions.end();
}

// Función de animación BFS
void bfsAnimation(sf::RenderWindow& window, const std::unordered_map<int, sf::Vector2f>& positions,
                  const std::unordered_map<int, std::vector<Edge>>& graph, int start, int target, const sf::Font& font) {
    std::queue<int> q;
    std::unordered_map<int, bool> visited;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Dibujar estado actual
        window.clear(sf::Color::Black);
        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                if (nodeExists(positions, node) && nodeExists(positions, edge.target)) {
                    drawEdge(window, positions.at(node), positions.at(edge.target));
                }
            }
        }

        for (const auto& [node, position] : positions) {
            if (node == current) {
                drawNode(window, position, std::to_string(node), font, sf::Color::Yellow);
            } else if (visited[node]) {
                drawNode(window, position, std::to_string(node), font, sf::Color::Green);
            } else {
                drawNode(window, position, std::to_string(node), font, sf::Color::Blue);
            }
        }

        drawTexto(window, "Modo: BFS", window.getSize().x / 2 - 50, 10, 30, font, sf::Color::White);
        drawTexto(window, "Buscando nodo: " + std::to_string(target), 10, 50, 20, font);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (current == target) {
            drawTexto(window, "Nodo encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Green);
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }

        if (graph.find(current) != graph.end()) {
            for (const auto& edge : graph.at(current)) {
                if (!visited[edge.target]) {
                    q.push(edge.target);
                    visited[edge.target] = true;
                }
            }
        }
    }

    drawTexto(window, "Nodo no encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Red);
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// Función de animación DFS
void dfsAnimation(sf::RenderWindow& window, const std::unordered_map<int, sf::Vector2f>& positions,
                  const std::unordered_map<int, std::vector<Edge>>& graph, int current, int target, const sf::Font& font, 
                  std::unordered_map<int, bool>& visited) {
    if (positions.find(current) == positions.end() || positions.find(target) == positions.end()) {
        throw std::runtime_error("Nodo inicial o final no está definido en el grafo.");
    }

    visited[current] = true;

    // Dibujar estado actual
    window.clear(sf::Color::Black);
    for (const auto& [node, edges] : graph) {
        for (const auto& edge : edges) {
            if (positions.find(node) != positions.end() && positions.find(edge.target) != positions.end()) {
                drawEdge(window, positions.at(node), positions.at(edge.target));
            }
        }
    }

    for (const auto& [node, position] : positions) {
        if (node == current) {
            drawNode(window, position, std::to_string(node), font, sf::Color::Yellow); // Nodo actual
        } else if (visited[node]) {
            drawNode(window, position, std::to_string(node), font, sf::Color::Green); // Nodos visitados
        } else {
            drawNode(window, position, std::to_string(node), font, sf::Color::Blue); // Nodos no visitados
        }
    }

    drawTexto(window, "Modo: DFS", window.getSize().x / 2 - 50, 10, 30, font, sf::Color::White);
    drawTexto(window, "Buscando nodo: " + std::to_string(target), 10, 50, 20, font);
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (current == target) {
        drawTexto(window, "Nodo encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Green);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    if (graph.find(current) != graph.end()) {
        for (const auto& edge : graph.at(current)) {
            if (!visited[edge.target]) {
                dfsAnimation(window, positions, graph, edge.target, target, font, visited);
                if (visited[target]) return; // Terminar si ya se encontró el nodo
            }
        }
    }
}

// Función principal para BFS y DFS (actualización)
void runGraphSearch(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("No se pudo cargar la fuente.");
    }

    std::unordered_map<int, sf::Vector2f> positions = {
        {0, {200, 400}}, {1, {400, 300}}, {2, {600, 400}},
        {3, {200, 500}}, {4, {400, 500}}, {5, {600, 500}}
    };

    std::unordered_map<int, std::vector<Edge>> graph = {
        {0, {{1}, {3}}}, {1, {{0}, {2}, {4}}}, {2, {{1}, {5}}},
        {3, {{0}, {4}}}, {4, {{1}, {3}, {5}}}, {5, {{2}, {4}}}
    };

    bool isBFS = true;
    std::string inputText;
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(static_cast<unsigned char>(event.text.unicode))) inputText += static_cast<char>(event.text.unicode);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace) {
                if (!inputText.empty()) inputText.pop_back();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
                return;  // Regresar al menú principal
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!inputText.empty()) {
                    int targetNode = std::stoi(inputText);
                    inputText.clear();
                    if (positions.find(targetNode) != positions.end()) {
                        if (isBFS) {
                            bfsAnimation(window, positions, graph, 0, targetNode, font);
                        } else {
                            std::unordered_map<int, bool> visited;
                            dfsAnimation(window, positions, graph, 0, targetNode, font, visited);
                        }
                    } else {
                        window.clear(sf::Color::Black);
                        drawTexto(window, "Nodo no valido. Intentalo de nuevo.", 10, 10, 20, font, sf::Color::Red);
                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) isBFS = !isBFS;
        }

        window.clear(sf::Color::Black);
        drawTexto(window, "Grafos", window.getSize().x / 2 - 50, 10, 40, font);
        drawTexto(window, isBFS ? "Modo: BFS" : "Modo: DFS", window.getSize().x / 2 - 50, 60, 30, font);
        drawTexto(window, "Ingrese el nodo objetivo y presione Enter", 10, 100, 20, font);
        drawTexto(window, "Presione Espacio para cambiar de modo", 10, 130, 20, font);
        drawTexto(window, "Entrada actual: " + inputText, 10, 160, 20, font);
        drawTexto(window, "Presione Shift para regresar al menu anterior " + inputText, 10, 190, 15, font);

        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                if (positions.find(node) != positions.end() && positions.find(edge.target) != positions.end()) {
                    drawEdge(window, positions.at(node), positions.at(edge.target));
                }
            }
        }

        for (const auto& [node, position] : positions) {
            drawNode(window, position, std::to_string(node), font, sf::Color::Blue);
        }

        window.display();
    }
}

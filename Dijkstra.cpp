#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <climits>

// Estructura para las aristas del grafo
struct Edge {
    int target;
    int weight;
};

// Función para dibujar texto en la ventana
void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size, const sf::Font& font, sf::Color color = sf::Color::White) {
    sf::Text sfText(text, font, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(color);
    sfText.setStyle(sf::Text::Bold);
    window.draw(sfText);
}

// Función para dibujar un nodo
void drawNode(sf::RenderWindow& window, const sf::Vector2f& position, const std::string& label, const sf::Font& font) {
    sf::CircleShape circle(20);
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setPosition(position - sf::Vector2f(20, 20));
    window.draw(circle);

    sf::Text text(label, font, 20);
    text.setPosition(position - sf::Vector2f(10, 10));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Función para dibujar una arista única con pesos ajustados
void drawEdge(sf::RenderWindow& window, const sf::Vector2f& from, const sf::Vector2f& to, int weight, const sf::Font& font, std::set<std::pair<int, int>>& drawnEdges, int nodeFrom, int nodeTo) {
    if (drawnEdges.find({nodeFrom, nodeTo}) != drawnEdges.end() || drawnEdges.find({nodeTo, nodeFrom}) != drawnEdges.end()) {
        return;
    }
    drawnEdges.insert({nodeFrom, nodeTo});

    sf::Vertex line[] = {
        sf::Vertex(from, sf::Color::White),
        sf::Vertex(to, sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);

    sf::Vector2f midPoint = (from + to) / 2.0f;
    sf::Vector2f direction = to - from;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitDirection = direction / length;
    sf::Vector2f perpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = perpendicular * 15.0f;
    sf::Text text(std::to_string(weight), font, 15);
    text.setPosition(midPoint + offset);
    text.setFillColor(sf::Color::Red);
    window.draw(text);
}

int generateRandomWeight() {
    return rand() % 10 + 1;
}

std::vector<int> dijkstra(const std::unordered_map<int, std::vector<Edge>>& graph, int start, int end) {
    std::unordered_map<int, int> distances;
    std::unordered_map<int, int> previous;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    for (const auto& [node, _] : graph) {
        distances[node] = INT_MAX;
        previous[node] = -1;
    }
    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();

        if (current == end) break;

        for (const auto& edge : graph.at(current)) {
            int newDist = distances[current] + edge.weight;
            if (newDist < distances[edge.target]) {
                distances[edge.target] = newDist;
                previous[edge.target] = current;
                pq.push({newDist, edge.target});
            }
        }
    }

    std::vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void runDijkstra(sf::RenderWindow& window) {
    bool backToMenu = true;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("No se pudo cargar la fuente.");
    }

    std::unordered_map<int, sf::Vector2f> positions = {
        {0, {150, 350}}, {1, {300, 300}}, {2, {450, 350}},
        {3, {150, 500}}, {4, {450, 500}}, {5, {600, 400}},
        {6, {300, 200}}, {7, {700, 300}}
    };

    std::unordered_map<int, std::vector<Edge>> graph = {
        {0, {{1, 2}, {3, 4}}},
        {1, {{0, 2}, {2, 5}, {4, 3}}},
        {2, {{1, 5}, {5, 2}}},
        {3, {{0, 4}, {4, 3}}},
        {4, {{1, 3}, {3, 3}, {5, 2}}},
        {5, {{2, 2}, {4, 2}, {7, 5}}},
        {6, {{1, 1}, {7, 6}}},
        {7, {{5, 5}, {6, 6}}}
    };

    int startNode = -1, endNode = -1;
    std::vector<int> path;
    sf::Event event;

    std::string infoText = "Ingrese nodo inicial (0-7)";
    std::string inputText = "";
    bool isStartNode = true;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                backToMenu = true;
                return;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace) {
                if (!inputText.empty()) {
                    inputText.pop_back();
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(static_cast<unsigned char>(event.text.unicode))) {
                    inputText += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (isStartNode) {
                    startNode = std::stoi(inputText);
                    infoText = "Ingrese nodo final (0-7) ";
                    inputText = "";
                    isStartNode = false;
                } else if (endNode == -1) {
                    endNode = std::stoi(inputText);
                    path = dijkstra(graph, startNode, endNode);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
                return;  // Regresar al menú principal
            }
        }

        window.clear(sf::Color::Black);
        drawText(window, "Dijkstra", window.getSize().x / 2 - 100, 10, 40, font);
        drawText(window, infoText, 10, 60, 25, font);
        drawText(window, "Entrada actual: " + inputText, 10, 100, 15, font);
        drawText(window, "Presione Shift para regresar al menu anterior", 10, 130, 20, font);

        std::set<std::pair<int, int>> drawnEdges;
        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                drawEdge(window, positions[node], positions[edge.target], edge.weight, font, drawnEdges, node, edge.target);
            }
        }

        for (const auto& [node, position] : positions) {
            drawNode(window, position, std::to_string(node), font);
        }

        if (!path.empty()) {
            std::string result = "Camino mas corto: ";
            for (int node : path) {
                result += std::to_string(node) + " -> ";
            }
            result = result.substr(0, result.size() - 4);
            drawText(window, result, 10, 550, 20, font);
        }

        window.display();
    }
}

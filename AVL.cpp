#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include "Utils.h"  // Asegúrate de que esta biblioteca esté bien configurada

// Estructura de un nodo AVL
struct AVLNode {
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
};

// Funciones auxiliares para AVL
int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Rotación
    x->right = y;
    y->left = T2;

    // Actualizar alturas
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Rotación
    y->left = x;
    x->right = T2;

    // Actualizar alturas
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* insertNode(AVLNode* node, int value) {
    if (!node)
        return new AVLNode(value);

    if (value < node->value)
        node->left = insertNode(node->left, value);
    else if (value > node->value)
        node->right = insertNode(node->right, value);
    else
        return node;

    // Actualizar altura del nodo actual
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Obtener el factor de balance
    int balance = getBalance(node);

    // Rotaciones para balancear
    if (balance > 1 && value < node->left->value)
        return rotateRight(node);
    if (balance < -1 && value > node->right->value)
        return rotateLeft(node);
    if (balance > 1 && value > node->left->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && value < node->right->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}
AVLNode* deleteNode(AVLNode* root, int value) {
    if (!root)
        return root;

    // Paso 1: Buscar el nodo a eliminar
    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    } else {
        // Nodo encontrado
        if (!root->left || !root->right) {
            // Caso 1 y 2: Nodo con 0 o 1 hijo
            AVLNode* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        } else {
            // Caso 3: Nodo con 2 hijos
            AVLNode* temp = root->right;
            while (temp->left) {
                temp = temp->left;
            }
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
    }

    // Paso 2: Actualizar altura
    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    // Paso 3: Rebalancear el árbol
    int balance = getBalance(root);

    // Izquierda pesada
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Derecha pesada
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Función para dibujar flechas
void drawArrow2(sf::RenderWindow& window, float startX, float startY, float endX, float endY) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(startX, startY), sf::Color::White),
        sf::Vertex(sf::Vector2f(endX, endY), sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);

    // Cabeza de la flecha
    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, sf::Vector2f(endX, endY));
    arrowHead.setPoint(1, sf::Vector2f(endX - 10, endY - 5));
    arrowHead.setPoint(2, sf::Vector2f(endX - 10, endY + 5));
    arrowHead.setFillColor(sf::Color::White);
    window.draw(arrowHead);
}

// Función para dibujar nodos en el árbol AVL
void drawNode(sf::RenderWindow& window, AVLNode* node, float x, float y, float xOffset, float yOffset, sf::Font& font) {
    if (!node) return;

    // Dibujar el nodo actual
    sf::CircleShape circle(20);
    circle.setFillColor(sf::Color::Red);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setPosition(x - 20, y - 20); // Centrar el círculo en (x, y)
    window.draw(circle);

    // Dibujar valor del nodo centrado
    sf::Text text(std::to_string(node->value), font, 20);
    text.setFillColor(sf::Color::White);
    text.setPosition(x - text.getLocalBounds().width / 2, y - text.getLocalBounds().height / 2 - 5); // Centrar texto
    window.draw(text);

    // Dibujar conexiones con los hijos
    if (node->left) {
        drawArrow2(window, x, y, x - xOffset, y + yOffset);
        drawNode(window, node->left, x - xOffset, y + yOffset, xOffset * 0.7, yOffset, font);
    }

    if (node->right) {
        drawArrow2(window, x, y, x + xOffset, y + yOffset);
        drawNode(window, node->right, x + xOffset, y + yOffset, xOffset * 0.7, yOffset, font);
    }
}

// Función principal para manejar la animación y operaciones AVL
void runAVL(sf::RenderWindow& window) {
    AVLNode* root = nullptr;
    std::string inputBuffer;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    std::string mode = "Insercion";
    bool insertionMode = true;

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(event.text.unicode)) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\b' && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                } else if (event.text.unicode == 13) { // Enter
                    if (inputBuffer.empty()) {
                        insertionMode = !insertionMode;
                        mode = insertionMode ? "Insercion" : "Eliminacion";
                    } else {
                        int value = std::stoi(inputBuffer);
                        if (insertionMode) {
                            root = insertNode(root, value);
                        } else {
                            root = deleteNode(root, value);
                        }
                        inputBuffer.clear();
                    }
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // Salir del bucle para regresar al menú principal
            }
        }

        // Dibujar fondo
        window.clear(sf::Color::Black);

        // Dibujar título centrado
        sf::Text title("Arboles AVL", font, 40);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        // Dibujar modo actual
        sf::Text modeText("Modo: " + mode, font, 25);
        modeText.setStyle(sf::Text::Bold);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 60);
        window.draw(modeText);

        // Dibujar instrucciones
        drawText(window, "Presione ENTER sin numero para cambiar de modo", 10, 110, 15);
        drawText(window, "Valor actual: " + inputBuffer, 10, 140, 15);
        drawText(window, "Presione ESC para regresar al menu", 10, 170, 15);

        // Dibujar el árbol AVL más abajo
        if (root) {
            drawNode(window, root, window.getSize().x / 2, 250, 150, 100, font); // Y inicial ajustada a 250
        }

        window.display();
    }
}
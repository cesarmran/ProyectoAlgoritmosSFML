#include "utils.h"

void drawText(sf::RenderWindow& window, const std::string& message, int x, int y, int size) {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        throw std::runtime_error("No se pudo cargar la fuente.");
    }
    sf::Text text;
    text.setFont(font);
    text.setString(message);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
    window.draw(text);
}

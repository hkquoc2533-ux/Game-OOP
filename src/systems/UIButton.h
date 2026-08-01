#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Nut bam don gian tai su dung cho tat ca cac Scene (MainMenu, Pause,
// CharacterSelect...). Gom RectangleShape + Text lai de khoi lap lai code
// ve nut o tung noi -> giu cau truc file don gian nhu yeu cau cua de bai.
class UIButton {
public:
    UIButton(sf::Font& font, const std::string& label,
             sf::Vector2f position, sf::Vector2f size)
        : shape(size), text(font, label, 22), hasTexture(false) {
        shape.setPosition(position);
        shape.setFillColor(sf::Color(60, 60, 90));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);

        text.setFillColor(sf::Color::White);
        centerText();
    }

    void setTexture(const sf::Texture* texture) {
        if (texture) {
            shape.setTexture(texture, true);
            shape.setFillColor(sf::Color::White);
            shape.setOutlineThickness(0.f);
            hasTexture = true;
            // Xoa text de tránh đè len hinh ảnh nút
            text.setString("");
        }
    }

    bool isHovered(sf::Vector2f mousePos) const {
        return shape.getGlobalBounds().contains(mousePos);
    }

    void draw(sf::RenderWindow& window) {
        if (hasTexture) {
            shape.setFillColor(isHoveredCache ? sf::Color(200, 200, 200) : sf::Color::White);
        } else {
            shape.setFillColor(isHoveredCache ? sf::Color(90, 90, 140) : sf::Color(60, 60, 90));
        }
        window.draw(shape);
        window.draw(text);
    }

    void updateHover(sf::Vector2f mousePos) {
        isHoveredCache = isHovered(mousePos);
    }

private:
    void centerText() {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                         bounds.position.y + bounds.size.y / 2.f});
        text.setPosition({shape.getPosition().x + shape.getSize().x / 2.f,
                           shape.getPosition().y + shape.getSize().y / 2.f});
    }

    sf::RectangleShape shape;
    sf::Text text;
    bool isHoveredCache = false;
    bool hasTexture = false;
};

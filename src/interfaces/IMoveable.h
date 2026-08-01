#pragma once
#include <SFML/System/Vector2.hpp>

// Interface (Truu tuong hoa - Abstraction)
// Danh cho nhung doi tuong co the di chuyen tren ban do (nhan vat, quai...)
class IMoveable {
public:
    virtual ~IMoveable() = default;

    virtual void move(const sf::Vector2f& direction, float deltaTime) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& position) = 0;
};

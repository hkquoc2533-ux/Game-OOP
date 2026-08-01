#pragma once
#include <SFML/Graphics.hpp>

// Interface (Truu tuong hoa - Abstraction)
// Moi doi tuong xuat hien va can cap nhat moi khung hinh trong game
// (nhan vat, quai, nut bam co animation, v.v.) deu phai cai dat interface nay.
class IGameObject {
public:
    virtual ~IGameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

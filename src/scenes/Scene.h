#pragma once
#include <SFML/Graphics.hpp>

class Game; // forward declaration, tranh include vong lap

// Interface (Truu tuong hoa) cho tat ca cac man hinh cua game:
// MainMenu, CharacterSelect, TowerMap, Combat, Pause...
// Game chi biet lam viec voi Scene* ma khong can quan tam chi tiet ben trong
// -> de dang them man hinh moi ma khong sua Game.h/.cpp (mo rong tot).
class Scene {
public:
    explicit Scene(Game& game) : game(game) {}
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

protected:
    Game& game;
};

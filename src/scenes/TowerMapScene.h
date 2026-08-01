#pragma once
#include "Scene.h"
#include "../systems/UIButton.h"

// Man hinh "ban do tang". Don gian hoa: hien ten tang hien tai + nut
// "Vao dau truong" de bat dau combat, nut "Pause".
// Tang 1,2: thang may bi khoa cho toi khi het quai (kiem tra qua GameSession
// sau khi CombatScene bao ve). Tang 3: chi co 1 quai to (boss).
class TowerMapScene : public Scene {
public:
    explicit TowerMapScene(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Text floorText;
    UIButton enterCombatButton;
    UIButton pauseButton;

    void refreshFloorText();
};

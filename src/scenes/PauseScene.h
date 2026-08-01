#pragma once
#include "Scene.h"
#include "../systems/UIButton.h"

// Man hinh Pause: hien khi nguoi choi bam nut Pause trong TowerMap/Combat.
// Cho phep: Tiep tuc, Luu game (5 slot), Thoat ve Menu chinh.
class PauseScene : public Scene {
public:
    explicit PauseScene(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Text title;
    UIButton resumeButton;
    UIButton saveButton;
    UIButton exitToMenuButton;
    sf::Text saveResultText;
    int nextSlotToUse = 0; // vi du don gian: luu vao slot ke tiep (0..4) roi quay vong
};

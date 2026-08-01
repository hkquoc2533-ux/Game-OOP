#pragma once
#include "Scene.h"
#include "../systems/UIButton.h"
#include <vector>

// Man hinh menu chinh: Play / Save (tiep tuc) / Quit
class MainMenuScene : public Scene {
public:
    explicit MainMenuScene(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Text title;
    UIButton playButton;
    UIButton continueButton;
    UIButton quitButton;
};

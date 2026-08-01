#include "TowerMapScene.h"
#include "../Game.h"
#include "../systems/TextureManager.h"
#include <string>

TowerMapScene::TowerMapScene(Game& game)
    : Scene(game),
      floorText(game.getFont(), "", 28),
      enterCombatButton(game.getFont(), "Vao dau truong", {250.f, 300.f}, {260.f, 60.f}),
      pauseButton(game.getFont(), "Pause", {670.f, 20.f}, {110.f, 45.f}) {
    floorText.setPosition({230.f, 150.f});
    floorText.setFillColor(sf::Color::White);
    refreshFloorText();
}

void TowerMapScene::refreshFloorText() {
    GameSession& session = game.getSession();
    int floor = session.currentFloor;

    if (session.lastInitializedFloor != floor) {
        session.lastInitializedFloor = floor;
        session.enemiesRemaining = (floor <= 2) ? 5 : 1; // tang 1-2: 5 quai thuong, tang 3: 1 boss
    }

    std::string floorLabel = (floor <= 2)
        ? ("Tang quai " + std::to_string(floor) + " / 3 - con lai " +
           std::to_string(session.enemiesRemaining) + " quai")
        : "Tang quai 3 / 3 - TRUM CUOI";
    floorText.setString("Toa Thap - " + floorLabel);
}

void TowerMapScene::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) return;
        sf::Vector2f pos = game.getMousePositionInWorld();

        if (enterCombatButton.isHovered(pos)) {
            game.changeScene(SceneId::Combat);
        } else if (pauseButton.isHovered(pos)) {
            game.getSession().pauseReturnScene = SceneId::TowerMap;
            game.changeScene(SceneId::Pause);
        }
    }
}

void TowerMapScene::update(float /*deltaTime*/) {
    refreshFloorText();
    sf::Vector2f mouseF = game.getMousePositionInWorld();
    enterCombatButton.updateHover(mouseF);
    pauseButton.updateHover(mouseF);
}

void TowerMapScene::draw(sf::RenderWindow& window) {
    if (const sf::Texture* bgTex = TextureManager::getTexture("Background/bg_tower.png")) {
        sf::Sprite bgSprite(*bgTex);
        sf::Vector2u size = bgTex->getSize();
        if (size.x > 0 && size.y > 0) {
            bgSprite.setScale({800.f / size.x, 600.f / size.y});
        }
        window.draw(bgSprite);
    }

    window.draw(floorText);
    enterCombatButton.draw(window);
    pauseButton.draw(window);
}

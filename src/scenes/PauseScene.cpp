#include "PauseScene.h"
#include "../Game.h"
#include "../systems/SaveManager.h"

PauseScene::PauseScene(Game& game)
    : Scene(game),
      title(game.getFont(), "TAM DUNG", 32),
      resumeButton(game.getFont(), "Tiep tuc", {300.f, 220.f}, {200.f, 55.f}),
      saveButton(game.getFont(), "Luu game", {300.f, 290.f}, {200.f, 55.f}),
      exitToMenuButton(game.getFont(), "Thoat ve Menu", {300.f, 360.f}, {200.f, 55.f}),
      saveResultText(game.getFont(), "", 18) {
    title.setPosition({320.f, 140.f});
    title.setFillColor(sf::Color::White);
    saveResultText.setPosition({300.f, 430.f});
    saveResultText.setFillColor(sf::Color::Green);
}

void PauseScene::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) return;
        sf::Vector2f pos = game.getMousePositionInWorld();

        if (resumeButton.isHovered(pos)) {
            game.changeScene(game.getSession().pauseReturnScene);
        } else if (saveButton.isHovered(pos)) {
            bool ok = SaveManager::saveGame(nextSlotToUse, game.getSession());
            saveResultText.setString(ok
                ? ("Da luu vao slot " + std::to_string(nextSlotToUse))
                : "Luu that bai!");
            nextSlotToUse = (nextSlotToUse + 1) % SaveManager::MAX_SLOTS;
        } else if (exitToMenuButton.isHovered(pos)) {
            game.changeScene(SceneId::MainMenu);
        }
    }
}

void PauseScene::update(float /*deltaTime*/) {
    sf::Vector2f mouseF = game.getMousePositionInWorld();
    resumeButton.updateHover(mouseF);
    saveButton.updateHover(mouseF);
    exitToMenuButton.updateHover(mouseF);
}

void PauseScene::draw(sf::RenderWindow& window) {
    window.draw(title);
    resumeButton.draw(window);
    saveButton.draw(window);
    exitToMenuButton.draw(window);
    window.draw(saveResultText);
}

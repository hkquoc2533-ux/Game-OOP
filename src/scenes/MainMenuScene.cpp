#include "MainMenuScene.h"
#include "../Game.h"
#include "../systems/SaveManager.h"

#include "../systems/TextureManager.h"

MainMenuScene::MainMenuScene(Game& game)
    : Scene(game),
      title(game.getFont(), "TOWER TURN-BASE", 40),
      playButton(game.getFont(), "Play", {300.f, 260.f}, {200.f, 60.f}),
      continueButton(game.getFont(), "Save/Load", {300.f, 340.f}, {200.f, 60.f}),
      quitButton(game.getFont(), "Quit", {300.f, 420.f}, {200.f, 60.f}) {
    title.setPosition({190.f, 120.f});
    title.setFillColor(sf::Color::White);

    playButton.setTexture(TextureManager::getTexture("Button/button_play.png"));
    continueButton.setTexture(TextureManager::getTexture("Button/button_setting.png"));
    quitButton.setTexture(TextureManager::getTexture("Button/button_quit.png"));
}

void MainMenuScene::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) return;
        sf::Vector2f pos = game.getMousePositionInWorld();

        if (playButton.isHovered(pos)) {
            game.changeScene(SceneId::CharacterSelect);
        } else if (continueButton.isHovered(pos)) {
            GameSession loaded;
            if (SaveManager::loadGame(0, loaded)) {
                game.getSession() = loaded;
                game.changeScene(SceneId::TowerMap);
            }
        } else if (quitButton.isHovered(pos)) {
            game.getWindow().close();
        }
    }
}

void MainMenuScene::update(float /*deltaTime*/) {
    sf::Vector2f mouseF = game.getMousePositionInWorld();
    playButton.updateHover(mouseF);
    continueButton.updateHover(mouseF);
    quitButton.updateHover(mouseF);
}

#include "../systems/TextureManager.h"

void MainMenuScene::draw(sf::RenderWindow& window) {
    if (const sf::Texture* bgTex = TextureManager::getTexture("Background/bg_main.png")) {
        sf::Sprite bgSprite(*bgTex);
        sf::Vector2u size = bgTex->getSize();
        if (size.x > 0 && size.y > 0) {
            bgSprite.setScale({800.f / size.x, 600.f / size.y});
        }
        window.draw(bgSprite);
    }

    window.draw(title);
    playButton.draw(window);
    continueButton.draw(window);
    quitButton.draw(window);
}

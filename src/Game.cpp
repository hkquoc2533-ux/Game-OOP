#include "Game.h"
#include "scenes/MainMenuScene.h"
#include "scenes/CharacterSelectScene.h"
#include "scenes/TowerMapScene.h"
#include "scenes/CombatScene.h"
#include "scenes/PauseScene.h"
#include "systems/Paths.h"
#include <stdexcept>

Game::Game()
    : window(sf::VideoMode({800u, 600u}), "Turn Base Tower", sf::Style::Default),
      gameView(sf::FloatRect({0.f, 0.f}, {800.f, 600.f})) {
    window.setFramerateLimit(60);
    updateView();

    const auto fontPath = Paths::Resolve("assets/fonts/font.ttf");
    if (!font.openFromFile(fontPath)) {
        throw std::runtime_error(
            "Khong the nap font tai " + fontPath.string() + ". "
            "Hay copy 1 file .ttf vao thu muc assets/fonts/ (canh file .exe) "
            "va doi ten thanh font.ttf.");
    }

    currentScene = createScene(SceneId::MainMenu);
}

void Game::updateView() {
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);
    if (windowWidth <= 0.f || windowHeight <= 0.f) return;

    float windowRatio = windowWidth / windowHeight;
    float viewRatio = 800.f / 600.f; // 4:3 aspect ratio
    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX = 0.f;
    float posY = 0.f;

    if (windowRatio >= viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    gameView.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
    window.setView(gameView);
}

sf::Vector2f Game::getMousePositionInWorld() const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePos, gameView);
}

std::unique_ptr<Scene> Game::createScene(SceneId id) {
    switch (id) {
        case SceneId::MainMenu:         return std::make_unique<MainMenuScene>(*this);
        case SceneId::CharacterSelect:  return std::make_unique<CharacterSelectScene>(*this);
        case SceneId::TowerMap:         return std::make_unique<TowerMapScene>(*this);
        case SceneId::Combat:           return std::make_unique<CombatScene>(*this);
        case SceneId::Pause:            return std::make_unique<PauseScene>(*this);
    }
    return std::make_unique<MainMenuScene>(*this);
}

void Game::changeScene(SceneId id) {
    currentScene = createScene(id);
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::Resized>()) {
                updateView();
            }
            currentScene->handleEvent(*event);
        }

        float deltaTime = clock.restart().asSeconds();
        currentScene->update(deltaTime);

        window.clear(sf::Color(25, 25, 35));
        window.setView(gameView);
        currentScene->draw(window);
        window.display();
    }
}

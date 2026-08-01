#pragma once
#include <memory>
#include <SFML/Graphics.hpp>   
#include "scenes/Scene.h"
#include "systems/GameSession.h"
#include "systems/SceneId.h"

// Lop trung tam: giu cua so SFML va scene hien tai, chay vong lap game.
// Ap dung Dong goi: cac scene chi tuong tac voi Game qua cac ham cong khai
// (changeScene, getWindow...), khong duoc dong window truc tiep.
class Game {
public:
    Game();

    void run();
    void changeScene(SceneId id);

    sf::RenderWindow& getWindow() { return window; }
    sf::Font& getFont() { return font; }
    GameSession& getSession() { return session; }

    sf::Vector2f getMousePositionInWorld() const;
    void updateView();

private:
    sf::RenderWindow window;
    sf::View gameView;
    sf::Font font;
    GameSession session;
    std::unique_ptr<Scene> currentScene;

    std::unique_ptr<Scene> createScene(SceneId id);
};

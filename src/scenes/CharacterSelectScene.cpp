#include "CharacterSelectScene.h"
#include "../Game.h"
#include "../systems/CharacterFactory.h"
#include "../systems/TextureManager.h"
#include <algorithm>

CharacterSelectScene::CharacterSelectScene(Game& game)
    : Scene(game),
      onePlayerButton(game.getFont(), "1 Player", {220.f, 250.f}, {180.f, 60.f}),
      twoPlayerButton(game.getFont(), "2 Player", {420.f, 250.f}, {180.f, 60.f}),
      infoText(game.getFont(), "Chon che do choi", 26) {
    infoText.setPosition({220.f, 150.f});
    infoText.setFillColor(sf::Color::White);

    const char* labels[4] = {"Dau Si", "Phap Su", "Xa Thu", "Do Don"};
    for (int i = 0; i < 4; ++i) {
        characterButtons.emplace_back(
            game.getFont(), labels[i],
            sf::Vector2f(150.f + i * 160.f, 320.f), sf::Vector2f(140.f, 60.f));
    }
}

bool CharacterSelectScene::alreadyChosen(CharacterType type) const {
    return std::find(leftChosen.begin(), leftChosen.end(), type) != leftChosen.end() ||
           std::find(rightChosen.begin(), rightChosen.end(), type) != rightChosen.end();
}

void CharacterSelectScene::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) return;
        sf::Vector2f pos = game.getMousePositionInWorld();

        if (step == 0) {
            if (onePlayerButton.isHovered(pos)) {
                step = 1;
                game.getSession().playerCount = 1;
                infoText.setString("Chon nhan vat cua ban");
            } else if (twoPlayerButton.isHovered(pos)) {
                step = 10; // 10 = che do 2P, ben trai chon nhan vat 1
                game.getSession().playerCount = 2;
                infoText.setString("Nguoi choi TRAI: chon nhan vat 1");
            }
            return;
        }

        for (size_t i = 0; i < characterButtons.size(); ++i) {
            if (!characterButtons[i].isHovered(pos)) continue;
            CharacterType type = static_cast<CharacterType>(i);
            if (alreadyChosen(type)) continue;

            if (step == 1) { // 1 Player - chon xong la vao thap luon
                leftChosen.push_back(type);
                finishSelection();
            } else if (step == 10) { // 2P trai - nhan vat 1
                leftChosen.push_back(type);
                step = 11;
                infoText.setString("Nguoi choi TRAI: chon nhan vat 2");
            } else if (step == 11) { // 2P trai - nhan vat 2
                leftChosen.push_back(type);
                step = 20;
                infoText.setString("Nguoi choi PHAI: chon nhan vat 1");
            } else if (step == 20) { // 2P phai - nhan vat 1
                rightChosen.push_back(type);
                step = 21;
                infoText.setString("Nguoi choi PHAI: chon nhan vat 2");
            } else if (step == 21) { // 2P phai - nhan vat 2
                rightChosen.push_back(type);
                finishSelection();
            }
            break;
        }
    }
}

void CharacterSelectScene::finishSelection() {
    GameSession& session = game.getSession();
    session.chosenCharacters.clear();
    for (auto t : leftChosen) session.chosenCharacters.push_back(t);
    for (auto t : rightChosen) session.chosenCharacters.push_back(t);
    session.currentFloor = 1;
    session.lastInitializedFloor = -1;

    session.playerHp.clear();
    session.playerRage.clear();
    for (auto t : session.chosenCharacters) {
        auto temp = CharacterFactory::create(t); // chi de biet maxHp mac dinh
        session.playerHp.push_back(temp->getMaxHp());
        session.playerRage.push_back(0);
    }

    game.changeScene(SceneId::TowerMap);
}

void CharacterSelectScene::update(float /*deltaTime*/) {
    sf::Vector2f mouseF = game.getMousePositionInWorld();
    onePlayerButton.updateHover(mouseF);
    twoPlayerButton.updateHover(mouseF);
    for (auto& b : characterButtons) b.updateHover(mouseF);
}

void CharacterSelectScene::draw(sf::RenderWindow& window) {
    if (const sf::Texture* bgTex = TextureManager::getTexture("Background/bg_character_select.png")) {
        sf::Sprite bgSprite(*bgTex);
        sf::Vector2u size = bgTex->getSize();
        if (size.x > 0 && size.y > 0) {
            bgSprite.setScale({800.f / size.x, 600.f / size.y});
        }
        window.draw(bgSprite);
    }

    window.draw(infoText);
    if (step == 0) {
        onePlayerButton.draw(window);
        twoPlayerButton.draw(window);
    } else {
        for (auto& b : characterButtons) b.draw(window);
    }
}

#pragma once
#include "Scene.h"
#include "../systems/UIButton.h"
#include "../systems/GameSession.h"
#include <vector>
#include <optional>

// Man hinh chon che do (1P/2P) va chon nhan vat.
// step = 0: chon 1P hay 2P
// step = 1: (1P) chon 1 nhan vat  |  (2P) ben trai chon nhan vat 1
// step = 2: (2P) ben trai chon nhan vat 2
// step = 3: (2P) ben phai chon nhan vat 1
// step = 4: (2P) ben phai chon nhan vat 2
class CharacterSelectScene : public Scene {
public:
    explicit CharacterSelectScene(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    int step = 0;
    UIButton onePlayerButton;
    UIButton twoPlayerButton;
    std::vector<UIButton> characterButtons; // Dau Si / Phap Su / Xa Thu / Do Don
    sf::Text infoText;

    std::vector<CharacterType> leftChosen;
    std::vector<CharacterType> rightChosen;

    bool alreadyChosen(CharacterType type) const;
    void finishSelection();
};

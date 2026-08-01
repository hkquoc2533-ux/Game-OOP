#include "CombatScene.h"
#include "../Game.h"
#include "../systems/CharacterFactory.h"
#include "../systems/CombatManager.h"
#include "../systems/TextureManager.h"
#include "../systems/Animator.h"
#include <algorithm>

CombatScene::CombatScene(Game& game)
    : Scene(game),
      basicAttackButton(game.getFont(), "Danh thuong", {60.f, 520.f}, {170.f, 50.f}),
      skill1Button(game.getFont(), "Chieu 1", {250.f, 520.f}, {150.f, 50.f}),
      skill2Button(game.getFont(), "Chieu 2", {420.f, 520.f}, {150.f, 50.f}),
      skill3Button(game.getFont(), "Chieu 3", {590.f, 520.f}, {150.f, 50.f}),
      pauseButton(game.getFont(), "Pause", {700.f, 20.f}, {90.f, 40.f}),
      logText(game.getFont(), "", 16),
      turnIndicator(game.getFont(), "", 20) {

    GameSession& session = game.getSession();

    for (size_t i = 0; i < session.chosenCharacters.size(); ++i) {
        auto character = CharacterFactory::create(session.chosenCharacters[i]);
        character->setCurrentFloor(session.currentFloor);
        character->scaleMaxHp(1.0f + (session.currentFloor - 1) * 0.20f);

        int hp = (i < session.playerHp.size()) ? session.playerHp[i] : character->getMaxHp();
        int rage = (i < session.playerRage.size()) ? session.playerRage[i] : 0;
        character->restoreState(hp, rage);
        character->setPosition({120.f + i * 140.f, 280.f});
        players.push_back(std::move(character));
    }

    spawnEnemyForCurrentFloor();

    logText.setPosition({30.f, 440.f});
    logText.setFillColor(sf::Color::Yellow);

    turnIndicator.setPosition({30.f, 20.f});
    turnIndicator.setFillColor(sf::Color::Cyan);
    updateTurnIndicator();

    appendLog("Tran chien bat dau!");
}

static void setupEnemyAnimations(Character& character, const std::string& prefix) {
    const sf::Texture* idleTex = TextureManager::getTexture(prefix + "_idle.png");
    const sf::Texture* attackTex = TextureManager::getTexture(prefix + "_attack.png");
    const sf::Texture* hurtTex = TextureManager::getTexture(prefix + "_hurt.png");

    if (idleTex) {
        character.addAnimation(AnimState::Idle, idleTex, 4, 0.15f, true);
    } else if (const sf::Texture* baseTex = TextureManager::getTexture(prefix + ".png")) {
        character.setTexture(*baseTex);
    }

    if (attackTex) {
        character.addAnimation(AnimState::Attack, attackTex, 6, 0.08f, false);
    }

    if (hurtTex) {
        character.addAnimation(AnimState::Hurt, hurtTex, 3, 0.1f, false);
    }
}

void CombatScene::spawnEnemyForCurrentFloor() {
    int floor = game.getSession().currentFloor;
    std::string prefix;
    if (floor == 1 || floor == 2) {
        int choice = rand() % 3;
        if (choice == 0) {
            enemy = std::make_unique<Enemy>("Rua Dot Bien", 60, 100, 12);
            prefix = "Enemy_1/enemy_1";
        } else if (choice == 1) {
            enemy = std::make_unique<Enemy>("Nguoi Cay Dien", 80, 100, 15);
            prefix = "Enemy_2/enemy_2";
        } else {
            enemy = std::make_unique<Enemy>("Bong Ma U Hon", 70, 100, 14);
            prefix = "Enemy_3/enemy_3";
        }
    } else {
        enemy = std::make_unique<BossEnemy>();
        prefix = "Boss/boss";
    }
    if (enemy) {
        enemy->setCurrentFloor(floor);
        enemy->scaleMaxHp(1.0f + (floor - 1) * 0.35f);
        setupEnemyAnimations(*enemy, prefix);
    }
    enemy->setPosition({650.f, 300.f});
}

void CombatScene::appendLog(const std::string& line) {
    battleLog += line + "\n";
    // chi giu 5 dong gan nhat de khong tran man hinh
    size_t linesToKeep = 5;
    size_t pos = 0, count = 0;
    for (size_t i = battleLog.size(); i-- > 0; ) {
        if (battleLog[i] == '\n') {
            if (++count == linesToKeep) { pos = i + 1; break; }
        }
    }
    if (count >= linesToKeep) battleLog = battleLog.substr(pos);
    logText.setString(battleLog);
}

void CombatScene::playerAction(int skillIndex) {
    if (battleOver || players.empty() || !enemy) return;
    Character& active = *players[activePlayerIndex];
    if (!active.isAlive()) return;

    active.playAnimation(AnimState::Attack);

    if (skillIndex == -1) {
        active.basicAttack(*enemy);
        appendLog(active.getName() + " danh thuong " + enemy->getName());
    } else {
        active.useSkill(skillIndex, *enemy);
        appendLog(active.getName() + " dung chieu " + std::to_string(skillIndex + 1));
    }

    checkBattleEnd();
    if (battleOver) return;

    enemyTurnIfAlive();
    checkBattleEnd();

    // Che do 2P: luan phien nhan vat dieu khien sau moi luot
    if (players.size() > 1) {
        activePlayerIndex = (activePlayerIndex + 1) % players.size();
        // bo qua nhan vat da guc, tim nguoi con song ke tiep
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[activePlayerIndex]->isAlive()) break;
            activePlayerIndex = (activePlayerIndex + 1) % players.size();
        }
    }
    updateTurnIndicator();
}

void CombatScene::updateTurnIndicator() {
    if (battleOver) {
        turnIndicator.setString("Ket thuc tran chien");
    } else if (!players.empty() && activePlayerIndex < players.size() && players[activePlayerIndex]->isAlive()) {
        turnIndicator.setString("Luot: " + players[activePlayerIndex]->getName() + " (" + std::to_string(activePlayerIndex + 1) + "P)");
    }
}

void CombatScene::enemyTurnIfAlive() {
    if (!enemy->isAlive()) return;
    // Quai chon 1 nguoi choi con song de tan cong (uu tien nguoi dang active)
    Character* target = players[activePlayerIndex]->isAlive()
        ? players[activePlayerIndex].get() : nullptr;
    if (!target) {
        for (auto& p : players) { if (p->isAlive()) { target = p.get(); break; } }
    }
    if (!target) return;

    enemy->playAnimation(AnimState::Attack);
    enemy->takeTurn(*target);
    appendLog(enemy->getName() + " tan cong " + target->getName());
}

void CombatScene::checkBattleEnd() {
    std::vector<Character*> playerPtrs;
    for (auto& p : players) playerPtrs.push_back(p.get());

    if (!enemy->isAlive()) {
        appendLog("Ha guc " + enemy->getName() + "!");
        battleOver = true;

        GameSession& session = game.getSession();
        session.enemiesRemaining = std::max(0, session.enemiesRemaining - 1);
        syncSessionState();

        if (session.enemiesRemaining <= 0) {
            session.currentFloor += 1;
            if (session.currentFloor > 3) {
                // Thang! quay ve menu chinh, co the choi lai tu dau
                game.changeScene(SceneId::MainMenu);
                return;
            }
        }
        game.changeScene(SceneId::TowerMap);
        return;
    }

    if (CombatManager::allPlayersDown(playerPtrs)) {
        appendLog("Ca doi guc! Choi lai tang nay tu dau.");
        battleOver = true;

        GameSession& session = game.getSession();
        // Thua -> lam lai tang hien tai tu dau: hoi day mau/no va reset so quai
        session.playerHp.assign(players.size(), 0);
        for (size_t i = 0; i < players.size(); ++i) {
            session.playerHp[i] = players[i]->getMaxHp();
        }
        session.playerRage.assign(players.size(), 0);
        session.lastInitializedFloor = -1; // ep TowerMapScene reset lai enemiesRemaining
        game.changeScene(SceneId::TowerMap);
    }
}

void CombatScene::syncSessionState() {
    GameSession& session = game.getSession();
    session.playerHp.assign(players.size(), 0);
    session.playerRage.assign(players.size(), 0);
    for (size_t i = 0; i < players.size(); ++i) {
        session.playerHp[i] = players[i]->getHp();
        session.playerRage[i] = players[i]->getRage();
    }
}

void CombatScene::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) return;
        sf::Vector2f pos = game.getMousePositionInWorld();

        if (pauseButton.isHovered(pos)) {
            syncSessionState();
            game.getSession().pauseReturnScene = SceneId::Combat;
            game.changeScene(SceneId::Pause);
            return;
        }
        if (battleOver) return;

        if (basicAttackButton.isHovered(pos)) playerAction(-1);
        else if (skill1Button.isHovered(pos)) playerAction(0);
        else if (skill2Button.isHovered(pos)) playerAction(1);
        else if (skill3Button.isHovered(pos)) playerAction(2);
    }
}

void CombatScene::update(float deltaTime) {
    for (auto& p : players) p->update(deltaTime);
    if (enemy) enemy->update(deltaTime);

    sf::Vector2f mouseF = game.getMousePositionInWorld();
    basicAttackButton.updateHover(mouseF);
    skill1Button.updateHover(mouseF);
    skill2Button.updateHover(mouseF);
    skill3Button.updateHover(mouseF);
    pauseButton.updateHover(mouseF);
}

// Ve mot thanh mau/no don gian bang RectangleShape (khong can anh)
static void drawBar(sf::RenderWindow& window, sf::Vector2f pos, float ratio, sf::Color color) {
    sf::RectangleShape background({100.f, 12.f});
    background.setPosition(pos);
    background.setFillColor(sf::Color(40, 40, 40));
    window.draw(background);

    sf::RectangleShape fill({100.f * std::max(0.f, std::min(1.f, ratio)), 12.f});
    fill.setPosition(pos);
    fill.setFillColor(color);
    window.draw(fill);
}

void CombatScene::draw(sf::RenderWindow& window) {
    int floor = game.getSession().currentFloor;
    std::string bgName = "Background/bg_combat_1.png";
    if (floor == 2) {
        bgName = "Background/bg_combat_2.png";
    } else if (floor >= 3) {
        bgName = "Background/bg_combat_boss.png";
    }

    if (const sf::Texture* bgTex = TextureManager::getTexture(bgName)) {
        sf::Sprite bgSprite(*bgTex);
        sf::Vector2u size = bgTex->getSize();
        if (size.x > 0 && size.y > 0) {
            bgSprite.setScale({800.f / size.x, 600.f / size.y});
        }
        window.draw(bgSprite);
    }

    window.draw(turnIndicator);

    for (size_t i = 0; i < players.size(); ++i) {
        auto& p = players[i];
        p->draw(window);
        drawBar(window, {p->getPosition().x - 50.f, p->getPosition().y - 40.f},
                float(p->getHp()) / float(p->getMaxHp()), sf::Color::Green);
        drawBar(window, {p->getPosition().x - 50.f, p->getPosition().y - 24.f},
                float(p->getRage()) / float(p->getMaxRage()), sf::Color(255, 140, 0));

        sf::Text nameLabel(game.getFont(), p->getName(), 14);
        nameLabel.setPosition({p->getPosition().x - 50.f, p->getPosition().y - 62.f});
        nameLabel.setFillColor(i == static_cast<size_t>(activePlayerIndex) ? sf::Color::Yellow : sf::Color::White);
        window.draw(nameLabel);
    }

    if (enemy) {
        enemy->draw(window);
        drawBar(window, {enemy->getPosition().x - 50.f, enemy->getPosition().y - 40.f},
                float(enemy->getHp()) / float(enemy->getMaxHp()), sf::Color::Red);
        drawBar(window, {enemy->getPosition().x - 50.f, enemy->getPosition().y - 24.f},
                float(enemy->getRage()) / float(enemy->getMaxRage()), sf::Color(255, 140, 0));

        sf::Text enemyLabel(game.getFont(), enemy->getName(), 14);
        enemyLabel.setPosition({enemy->getPosition().x - 50.f, enemy->getPosition().y - 62.f});
        enemyLabel.setFillColor(sf::Color::Red);
        window.draw(enemyLabel);
    }

    basicAttackButton.draw(window);
    skill1Button.draw(window);
    skill2Button.draw(window);
    skill3Button.draw(window);
    pauseButton.draw(window);
    window.draw(logText);
}

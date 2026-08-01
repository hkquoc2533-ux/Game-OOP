#pragma once
#include "Scene.h"
#include "../systems/UIButton.h"
#include "../entities/Character.h"
#include "../entities/Enemy.h"
#include "../entities/BossEnemy.h"
#include <memory>
#include <vector>

// Man hinh chien dau theo luot: nhan vat nguoi choi ben trai, quai ben phai.
// Tren moi nhan vat/quai co thanh mau va thanh no; duoi co 1 nut danh thuong
// va 3 nut chieu. Nguoi choi bam nut -> quai danh tra -> lap lai cho toi khi
// mot ben "chet" het.
class CombatScene : public Scene {
public:
    explicit CombatScene(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    std::vector<std::unique_ptr<Character>> players; // 1 hoac 2 nhan vat
    std::unique_ptr<Enemy> enemy;                     // dang danh 1 quai / lan

    int activePlayerIndex = 0; // nhan vat dang duoc dieu khien (che do 2P luan phien)

    UIButton basicAttackButton;
    UIButton skill1Button;
    UIButton skill2Button;
    UIButton skill3Button;
    UIButton pauseButton;

    sf::Text logText;
    sf::Text turnIndicator;
    std::string battleLog;

    bool battleOver = false;

    void spawnEnemyForCurrentFloor();
    void playerAction(int skillIndex); // -1 = danh thuong, 0..2 = chieu
    void enemyTurnIfAlive();
    void checkBattleEnd();
    void syncSessionState(); // luu mau/no cua tung nguoi choi lai vao GameSession
    void appendLog(const std::string& line);
    void updateTurnIndicator();
};

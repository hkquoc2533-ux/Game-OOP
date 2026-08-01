#pragma once
#include <vector>
#include "CharacterType.h"
#include "SceneId.h"

// Du lieu dung chung giua cac Scene trong suot mot luot choi:
// che do 1P/2P, nhan vat da chon, dang o tang may cua thap.
// Game so huu doi tuong nay va truyen tham chieu cho tung Scene can dung.
struct GameSession {
    int playerCount = 1;                       // 1 hoac 2
    std::vector<CharacterType> chosenCharacters; // 1 phan tu (1P) hoac 2 phan tu (2P)
    int currentFloor = 1;                       // 1, 2 hoac 3
    int saveSlot = -1;                          // -1 = chua gan slot nao

    // Mau/no cua tung nhan vat duoc luu lai giua cac tran danh trong cung
    // mot tang (cung chi so voi chosenCharacters). Duoc nap lai vao Character
    // qua Character::restoreState() moi khi vao CombatScene.
    std::vector<int> playerHp;
    std::vector<int> playerRage;

    int enemiesRemaining = 0;   // con lai bao nhieu quai o tang hien tai
    int lastInitializedFloor = -1; // TowerMapScene dung de biet khi nao can reset enemiesRemaining

    SceneId pauseReturnScene = SceneId::TowerMap; // scene de quay ve khi resume tu Pause
};

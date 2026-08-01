#pragma once
#include <vector>
#include "../entities/Character.h"
#include "../entities/Enemy.h"

// Gom cac ham kiem tra dieu kien thang/thua dung chung cho CombatScene,
// tach rieng khoi logic ve/nhan su kien de CombatScene khong bi phinh to.
class CombatManager {
public:
    static bool allDefeated(const std::vector<Enemy*>& enemies);
    static bool allPlayersDown(const std::vector<Character*>& players);
};

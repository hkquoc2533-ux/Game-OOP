#include "CombatManager.h"

bool CombatManager::allDefeated(const std::vector<Enemy*>& enemies) {
    for (auto* e : enemies) {
        if (e->isAlive()) return false;
    }
    return true;
}

bool CombatManager::allPlayersDown(const std::vector<Character*>& players) {
    for (auto* p : players) {
        if (p->isAlive()) return false;
    }
    return true;
}

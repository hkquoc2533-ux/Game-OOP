#include "BossEnemy.h"

BossEnemy::BossEnemy() : Enemy("Chua Te Tang Thap", /*maxHp*/ 400, /*maxRage*/ 100, /*attackDamage*/ 25) {}

void BossEnemy::takeTurn(Character& target) {
    ++turnCount;
    if (getRage() >= 50) {
        useSkill(0, target); // don manh
    } else if (turnCount % 3 == 0) {
        // Moi 3 luot, boss lam mot don danh kep
        basicAttack(target);
        basicAttack(target);
    } else {
        basicAttack(target);
    }
}

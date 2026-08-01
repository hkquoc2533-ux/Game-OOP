#include "Tank.h"
#include <algorithm>

Tank::Tank() : Character("Do Don", /*maxHp*/ 200, /*maxRage*/ 100, /*speed*/ 90.f) {}

void Tank::basicAttack(Character& target) {
    target.takeDamage(10);
    gainRage(18);
}

void Tank::useSkill(int skillIndex, Character& target) {
    switch (skillIndex) {
        case 0: // Ne don - danh nhe + hoi mau
            if (spendRage(20)) { target.takeDamage(10); heal(15); }
            break;
        case 1: // Dung khien - giam sat thuong luot toi (khong danh)
            if (spendRage(35)) shield = 15;
            break;
        case 2: // Phan don - sat thuong cao + hoi no
            if (spendRage(55)) { target.takeDamage(30); gainRage(10); }
            break;
        default: break;
    }
}

void Tank::applyIncomingDamageReduction(int& incomingDamage) {
    if (shield > 0) {
        incomingDamage = std::max(0, incomingDamage - shield);
        shield = 0;
    }
}

void Tank::takeDamage(int amount) {
    applyIncomingDamageReduction(amount);
    Character::takeDamage(amount);
}

#include "Archer.h"

Archer::Archer() : Character("Xa Thu", /*maxHp*/ 110, /*maxRage*/ 100, /*speed*/ 140.f) {}

void Archer::basicAttack(Character& target) {
    target.takeDamage(14);
    gainRage(22);
}

void Archer::useSkill(int skillIndex, Character& target) {
    switch (skillIndex) {
        case 0: // Ban lien hoan (2 phat nho)
            if (spendRage(25)) { target.takeDamage(14); target.takeDamage(14); }
            break;
        case 1: // Mui ten xuyen giap
            if (spendRage(45)) target.takeDamage(38);
            break;
        case 2: // Mua ten
            if (spendRage(65)) target.takeDamage(55);
            break;
        default: break;
    }
}

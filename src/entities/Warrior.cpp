#include "Warrior.h"

Warrior::Warrior() : Character("Dau Si", /*maxHp*/ 150, /*maxRage*/ 100, /*speed*/ 120.f) {}

void Warrior::basicAttack(Character& target) {
    target.takeDamage(15);
    gainRage(15);
}

void Warrior::useSkill(int skillIndex, Character& target) {
    switch (skillIndex) {
        case 0: // Cham lien hoan - no thap, sat thuong vua
            if (spendRage(25)) target.takeDamage(25);
            break;
        case 1: // Da manh - no vua, sat thuong cao
            if (spendRage(45)) target.takeDamage(40);
            break;
        case 2: // Cuong no - no cao, sat thuong rat cao + tu hoi mau nho
            if (spendRage(70)) { target.takeDamage(60); heal(10); }
            break;
        default: break;
    }
}

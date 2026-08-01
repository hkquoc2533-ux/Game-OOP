#include "Mage.h"

Mage::Mage() : Character("Phap Su", /*maxHp*/ 90, /*maxRage*/ 100, /*speed*/ 110.f) {}

void Mage::basicAttack(Character& target) {
    target.takeDamage(12);
    gainRage(20);
}

void Mage::useSkill(int skillIndex, Character& target) {
    switch (skillIndex) {
        case 0: // Cau lua nho
            if (spendRage(30)) target.takeDamage(35);
            break;
        case 1: // Bang gia - sat thuong + gia dinh (mo phong bang giam no doi phuong khong can thiet)
            if (spendRage(50)) target.takeDamage(45);
            break;
        case 2: // Thien thach - no cao, sat thuong cuc cao
            if (spendRage(80)) target.takeDamage(80);
            break;
        default: break;
    }
}

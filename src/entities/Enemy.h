#pragma once
#include "Character.h"

// Quai thuong (dung cho tang 1 va tang 2). Ke thua Character nen dung chung
// he thong mau/no/danh don voi nhan vat nguoi choi -> tai su dung code (Inheritance).
class Enemy : public Character {
public:
    Enemy(std::string name, int maxHp, int maxRage, int attackDamage);

    void basicAttack(Character& target) override;
    void useSkill(int skillIndex, Character& target) override;
    std::string getClassName() const override { return "Quai"; }

    // AI don gian: quai tu chon hanh dong cua no trong luot cua no
    virtual void takeTurn(Character& target);

protected:
    int attackDamage;
};

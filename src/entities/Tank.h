#pragma once
#include "Character.h"

// Do don: mau rat cao, sat thuong thap, co ky nang tu hoi mau / khien
class Tank : public Character {
public:
    Tank();

    void basicAttack(Character& target) override;
    void useSkill(int skillIndex, Character& target) override;
    std::string getClassName() const override { return "Do Don"; }
    void takeDamage(int amount) override;

private:
    int shield = 0; // giam sat thuong nhan vao o luot ke tiep
public:
    void applyIncomingDamageReduction(int& incomingDamage);
};

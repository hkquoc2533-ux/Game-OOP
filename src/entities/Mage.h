#pragma once
#include "Character.h"

// Phap su: mau thap, sat thuong phep cao, tieu hao no nhieu
class Mage : public Character {
public:
    Mage();

    void basicAttack(Character& target) override;
    void useSkill(int skillIndex, Character& target) override;
    std::string getClassName() const override { return "Phap Su"; }
};

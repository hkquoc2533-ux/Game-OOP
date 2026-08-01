#pragma once
#include "Character.h"

// Dau si: mau cao, sat thuong vat ly on dinh
class Warrior : public Character {
public:
    Warrior();

    void basicAttack(Character& target) override;
    void useSkill(int skillIndex, Character& target) override;
    std::string getClassName() const override { return "Dau Si"; }
};

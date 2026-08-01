#pragma once
#include "Character.h"

// Xa thu: mau trung binh, sat thuong deu, hoi no nhanh
class Archer : public Character {
public:
    Archer();

    void basicAttack(Character& target) override;
    void useSkill(int skillIndex, Character& target) override;
    std::string getClassName() const override { return "Xa Thu"; }
};

#pragma once
#include "Enemy.h"

// Quai to o tang 3 (tang cuoi). Da hinh: ghi de takeTurn() de co nhieu giai
// doan tan cong hon quai thuong.
class BossEnemy : public Enemy {
public:
    BossEnemy();

    void takeTurn(Character& target) override;
    std::string getClassName() const override { return "Trum Cuoi"; }

private:
    int turnCount = 0;
};

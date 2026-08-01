#include "Enemy.h"

Enemy::Enemy(std::string name_, int maxHp_, int maxRage_, int attackDamage_)
    : Character(std::move(name_), maxHp_, maxRage_, 80.f), attackDamage(attackDamage_) {}

void Enemy::basicAttack(Character& target) {
    target.takeDamage(attackDamage);
    gainRage(15);
}

void Enemy::useSkill(int /*skillIndex*/, Character& target) {
    // Quai thuong chi co 1 don manh hon khi du no
    if (spendRage(50)) target.takeDamage(attackDamage * 2);
}

void Enemy::takeTurn(Character& target) {
    // AI don gian: du no thi dung chieu, khong thi danh thuong
    if (getRage() >= 50) useSkill(0, target);
    else basicAttack(target);
}

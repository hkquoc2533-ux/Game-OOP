#pragma once
#include <memory>
#include "CharacterType.h"
#include "../entities/Character.h"

// Factory don gian: tao doi tuong Character* tu CharacterType.
// Giup CharacterSelectScene / CombatScene / SaveManager khong can biet chi
// tiet class con nao dang duoc dung (chi lam viec qua con tro Character*).
class CharacterFactory {
public:
    static std::unique_ptr<Character> create(CharacterType type);
};

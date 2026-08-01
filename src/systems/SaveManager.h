#pragma once
#include <string>
#include "GameSession.h"

// Quan ly luu/doc tien trinh choi ra file text don gian trong thu muc saves/.
// Toi da 5 slot (0..4). Dinh dang moi dong: key=value de de doc/sua bang tay.
class SaveManager {
public:
    static constexpr int MAX_SLOTS = 5;

    static bool saveGame(int slot, const GameSession& session);
    static bool loadGame(int slot, GameSession& outSession);
    static bool slotExists(int slot);
    static bool deleteSlot(int slot);
    static bool copySlot(int fromSlot, int toSlot); // "sao chep ban save ra nhieu ban khac"

private:
    static std::string slotPath(int slot);
};

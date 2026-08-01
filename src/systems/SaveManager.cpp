#include "SaveManager.h"
#include "Paths.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string SaveManager::slotPath(int slot) {
    // Duong dan tuyet doi toi thu muc saves/ canh file .exe, khong con phu
    // thuoc vao thu muc lam viec hien tai (xem giai thich trong Paths.h).
    return Paths::Resolve("saves/slot" + std::to_string(slot) + ".txt").string();
}

bool SaveManager::saveGame(int slot, const GameSession& session) {
    if (slot < 0 || slot >= MAX_SLOTS) return false;
    std::filesystem::create_directories(Paths::Resolve("saves"));

    std::ofstream out(slotPath(slot));
    if (!out.is_open()) return false;

    out << "playerCount=" << session.playerCount << "\n";
    out << "currentFloor=" << session.currentFloor << "\n";
    out << "characterCount=" << session.chosenCharacters.size() << "\n";
    for (size_t i = 0; i < session.chosenCharacters.size(); ++i) {
        out << "character" << i << "=" << static_cast<int>(session.chosenCharacters[i]) << "\n";
    }
    out << "enemiesRemaining=" << session.enemiesRemaining << "\n";
    out << "lastInitializedFloor=" << session.lastInitializedFloor << "\n";
    for (size_t i = 0; i < session.playerHp.size(); ++i) {
        out << "hp" << i << "=" << session.playerHp[i] << "\n";
        out << "rage" << i << "=" << session.playerRage[i] << "\n";
    }
    return true;
}

bool SaveManager::loadGame(int slot, GameSession& outSession) {
    if (!slotExists(slot)) return false;

    std::ifstream in(slotPath(slot));
    std::string line;
    int characterCount = 0;
    outSession.chosenCharacters.clear();

    while (std::getline(in, line)) {
        auto eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;
        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        if (key == "playerCount") outSession.playerCount = std::stoi(value);
        else if (key == "currentFloor") outSession.currentFloor = std::stoi(value);
        else if (key == "characterCount") characterCount = std::stoi(value);
        else if (key == "enemiesRemaining") outSession.enemiesRemaining = std::stoi(value);
        else if (key == "lastInitializedFloor") outSession.lastInitializedFloor = std::stoi(value);
        else if (key.rfind("character", 0) == 0) {
            outSession.chosenCharacters.push_back(static_cast<CharacterType>(std::stoi(value)));
        } else if (key.rfind("hp", 0) == 0) {
            size_t idx = std::stoi(key.substr(2));
            if (outSession.playerHp.size() <= idx) outSession.playerHp.resize(idx + 1, 0);
            outSession.playerHp[idx] = std::stoi(value);
        } else if (key.rfind("rage", 0) == 0) {
            size_t idx = std::stoi(key.substr(4));
            if (outSession.playerRage.size() <= idx) outSession.playerRage.resize(idx + 1, 0);
            outSession.playerRage[idx] = std::stoi(value);
        }
    }
    outSession.saveSlot = slot;
    (void)characterCount; // du lieu tham khao, cac dong "characterN" da nap du roi
    return true;
}

bool SaveManager::slotExists(int slot) {
    if (slot < 0 || slot >= MAX_SLOTS) return false;
    return std::filesystem::exists(slotPath(slot));
}

bool SaveManager::deleteSlot(int slot) {
    if (!slotExists(slot)) return false;
    return std::filesystem::remove(slotPath(slot));
}

bool SaveManager::copySlot(int fromSlot, int toSlot) {
    if (!slotExists(fromSlot) || toSlot < 0 || toSlot >= MAX_SLOTS) return false;
    std::filesystem::copy_file(
        slotPath(fromSlot), slotPath(toSlot),
        std::filesystem::copy_options::overwrite_existing);
    return true;
}

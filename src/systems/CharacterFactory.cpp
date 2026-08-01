#include "CharacterFactory.h"
#include "../entities/Warrior.h"
#include "../entities/Mage.h"
#include "../entities/Archer.h"
#include "../entities/Tank.h"

#include "TextureManager.h"
#include "Animator.h"

static void setupCharacterAnimations(Character& character, const std::string& prefix) {
    const sf::Texture* idleTex = TextureManager::getTexture(prefix + "_idle.png");
    const sf::Texture* attackTex = TextureManager::getTexture(prefix + "_attack.png");
    const sf::Texture* hurtTex = TextureManager::getTexture(prefix + "_hurt.png");

    if (idleTex) {
        character.addAnimation(AnimState::Idle, idleTex, 4, 0.15f, true);
    } else if (const sf::Texture* baseTex = TextureManager::getTexture(prefix + ".png")) {
        character.setTexture(*baseTex);
    }

    if (attackTex) {
        character.addAnimation(AnimState::Attack, attackTex, 6, 0.08f, false);
    }

    if (hurtTex) {
        character.addAnimation(AnimState::Hurt, hurtTex, 3, 0.1f, false);
    }
}

std::unique_ptr<Character> CharacterFactory::create(CharacterType type) {
    std::unique_ptr<Character> character;
    std::string prefix;

    switch (type) {
        case CharacterType::Warrior:
            character = std::make_unique<Warrior>();
            prefix = "Warrior/warrior";
            break;
        case CharacterType::Mage:
            character = std::make_unique<Mage>();
            prefix = "Mage/mage";
            break;
        case CharacterType::Archer:
            character = std::make_unique<Archer>();
            prefix = "Archer/archer";
            break;
        case CharacterType::Tank:
            character = std::make_unique<Tank>();
            prefix = "Tank/tank";
            break;
    }

    if (character) {
        setupCharacterAnimations(*character, prefix);
    }

    return character;
}

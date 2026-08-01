#include "Character.h"
#include <algorithm>

Character::Character(std::string name_, int maxHp_, int maxRage_, float moveSpeed_)
    : name(std::move(name_)), maxHp(maxHp_), currentHp(maxHp_),
      maxRage(maxRage_), currentRage(0), moveSpeed(moveSpeed_) {
}

void Character::update(float deltaTime) {
    animator.update(deltaTime, sprite.get());

    if (animator.getCurrentState() == AnimState::Attack) {
        animOffsetTimer += deltaTime;
        // Lunges forward during attack
        float lunge = std::sin(animOffsetTimer * 15.f) * 30.f;
        visualOffset = {lunge, 0.f};
    } else if (animator.getCurrentState() == AnimState::Hurt) {
        animOffsetTimer += deltaTime;
        // Shakes when hit
        float shake = (static_cast<int>(animOffsetTimer * 100.f) % 2 == 0) ? 5.f : -5.f;
        visualOffset = {shake, 0.f};
    } else {
        animOffsetTimer = 0.f;
        visualOffset = {0.f, 0.f};
    }

    if (sprite) {
        sprite->setPosition(position + visualOffset);
        if (animator.getCurrentState() == AnimState::Hurt) {
            sprite->setColor(sf::Color::Red);
        } else {
            sprite->setColor(sf::Color::White);
        }
    }
}

void Character::draw(sf::RenderWindow& window) {
    if (sprite) {
        window.draw(*sprite);
    } else {
        // Placeholder hinh chu nhat khi chua co sprite (lunges/shakes when acting)
        sf::RectangleShape placeholder({50.f, 80.f});
        placeholder.setPosition({position.x - 25.f + visualOffset.x, position.y + visualOffset.y});
        if (animator.getCurrentState() == AnimState::Hurt) {
            placeholder.setFillColor(sf::Color::Red);
        } else if (animator.getCurrentState() == AnimState::Attack) {
            placeholder.setFillColor(sf::Color::Yellow);
        } else {
            placeholder.setFillColor(sf::Color(100, 150, 200));
        }
        placeholder.setOutlineColor(sf::Color::White);
        placeholder.setOutlineThickness(2.f);
        window.draw(placeholder);
    }
}

void Character::move(const sf::Vector2f& direction, float deltaTime) {
    position += direction * moveSpeed * deltaTime;
}

sf::Vector2f Character::getPosition() const {
    return position;
}

void Character::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void Character::setTexture(const sf::Texture& texture) {
    addAnimation(AnimState::Idle, &texture, 1, 0.12f, true);
}

void Character::addAnimation(AnimState state, const sf::Texture* texture, int frameCount, float frameDuration, bool loop) {
    if (!texture) return;
    if (!sprite) {
        sprite = std::make_unique<sf::Sprite>(*texture);
    }
    animator.addTrack(state, texture, frameCount, frameDuration, loop);
}

void Character::playAnimation(AnimState state) {
    animOffsetTimer = 0.f;
    animator.play(state);
}

void Character::takeDamage(int amount) {
    float dmgScale = 1.0f;
    if (getClassName() == "Quai" || getClassName() == "Trum Cuoi") {
        // Quái nhận sát thương từ người chơi (sát thương người chơi tăng 15% mỗi tầng)
        dmgScale = 1.0f + (currentFloor - 1) * 0.15f;
    } else {
        // Người chơi nhận sát thương từ quái (sát thương quái tăng 25% mỗi tầng)
        dmgScale = 1.0f + (currentFloor - 1) * 0.25f;
    }
    int finalDamage = static_cast<int>(amount * dmgScale);
    currentHp = std::max(0, currentHp - finalDamage);
    playAnimation(AnimState::Hurt);
}

void Character::scaleMaxHp(float factor) {
    if (factor <= 0.f) return;
    int oldMax = maxHp;
    maxHp = static_cast<int>(maxHp * factor);
    if (oldMax > 0) {
        currentHp = static_cast<int>(currentHp * (static_cast<float>(maxHp) / oldMax));
    } else {
        currentHp = maxHp;
    }
}

void Character::heal(int amount) {
    currentHp = std::min(maxHp, currentHp + amount);
}

void Character::gainRage(int amount) {
    currentRage = std::min(maxRage, currentRage + amount);
}

bool Character::spendRage(int amount) {
    if (currentRage < amount) return false;
    currentRage -= amount;
    return true;
}

void Character::restoreState(int hp, int rage) {
    currentHp = std::clamp(hp, 0, maxHp);
    currentRage = std::clamp(rage, 0, maxRage);
}

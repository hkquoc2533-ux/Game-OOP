#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

enum class AnimState {
    Idle,
    Attack,
    Hurt,
    Dead
};

struct AnimationTrack {
    const sf::Texture* texture = nullptr;
    int frameCount = 1;
    float frameDuration = 0.12f; // thoi gian 1 frame (giay)
    bool loop = true;
    sf::Vector2i frameSize = {0, 0}; // kich thuoc 1 frame (width, height)
};

class Animator {
public:
    Animator() = default;

    void addTrack(AnimState state, const sf::Texture* texture, int frameCount = 1, float frameDuration = 0.12f, bool loop = true);
    void play(AnimState state);
    void update(float deltaTime, sf::Sprite* sprite);

    AnimState getCurrentState() const { return currentState; }
    bool isPlaying(AnimState state) const { return currentState == state; }

private:
    std::unordered_map<AnimState, AnimationTrack> tracks;
    AnimState currentState = AnimState::Idle;
    int currentFrame = 0;
    float elapsedTime = 0.f;
};

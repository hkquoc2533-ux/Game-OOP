#include "Animator.h"

void Animator::addTrack(AnimState state, const sf::Texture* texture, int frameCount, float frameDuration, bool loop) {
    if (!texture) return;
    AnimationTrack track;
    track.texture = texture;
    track.frameCount = (frameCount > 0) ? frameCount : 1;
    track.frameDuration = frameDuration;
    track.loop = loop;

    sf::Vector2u texSize = texture->getSize();
    int frameW = static_cast<int>(texSize.x) / track.frameCount;
    int frameH = static_cast<int>(texSize.y);
    track.frameSize = {frameW, frameH};

    tracks[state] = track;
}

void Animator::play(AnimState state) {
    auto it = tracks.find(state);
    if (it != tracks.end()) {
        if (currentState != state || !it->second.loop) {
            currentState = state;
            currentFrame = 0;
            elapsedTime = 0.f;
        }
    }
}

void Animator::update(float deltaTime, sf::Sprite* sprite) {
    auto it = tracks.find(currentState);
    if (it == tracks.end() || !it->second.texture) {
        // Neu khong tim thay track hien tai, thu ve Idle
        if (currentState != AnimState::Idle) {
            currentState = AnimState::Idle;
            currentFrame = 0;
            elapsedTime = 0.f;
            it = tracks.find(AnimState::Idle);
        }
    }

    if (it == tracks.end() || !it->second.texture) return;
    const AnimationTrack& track = it->second;

    if (sprite) {
        sprite->setTexture(*track.texture);
        int frameW = track.frameSize.x;
        int frameH = track.frameSize.y;
        sprite->setTextureRect(sf::IntRect({currentFrame * frameW, 0}, {frameW, frameH}));
        sprite->setOrigin({frameW / 2.f, frameH / 2.f});
    }

    elapsedTime += deltaTime;
    if (elapsedTime >= track.frameDuration) {
        elapsedTime = 0.f;
        currentFrame++;
        if (currentFrame >= track.frameCount) {
            if (track.loop) {
                currentFrame = 0;
            } else {
                // Trở về Idle khi hoạt ảnh đánh / trúng đòn kết thúc
                currentState = AnimState::Idle;
                currentFrame = 0;
            }
        }
    }
}

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

// Manager quan ly va cache sf::Texture.
// Tranh viec sf::Texture bi huydiet khi sf::Sprite van dang su dung.
class TextureManager {
public:
    static const sf::Texture* getTexture(const std::string& filename);
    static void clear();

private:
    static std::unordered_map<std::string, sf::Texture> textures;
};

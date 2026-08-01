#include "TextureManager.h"
#include "Paths.h"
#include <iostream>

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

const sf::Texture* TextureManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return &it->second;
    }

    auto fullPath = Paths::Resolve("assets/images/" + filename);
    sf::Texture texture;
    if (texture.loadFromFile(fullPath.string())) {
        textures[filename] = std::move(texture);
        return &textures[filename];
    }

    // Neu khong nap duoc anh, tra ve nullptr de Character tu ve hinh chu nhat placeholder
    return nullptr;
}

void TextureManager::clear() {
    textures.clear();
}

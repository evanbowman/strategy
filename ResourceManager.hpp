#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <array>

enum class RID {
    Tileset,
    OceanBkg,
    Cursor,
    Count
};

class ResourceManager {
public:
    ResourceManager();
    
    template <RID id>
    sf::Sprite & GetSprite() {
	return std::get<static_cast<int>(id)>(m_sprites);
    }
    
private:
    std::array<sf::Sprite, static_cast<int>(RID::Count)> m_sprites;
    std::array<sf::Texture, static_cast<int>(RID::Count)> m_textures;

    template <RID id>
    void SetupDrawable(const std::string & path) {
	if (!std::get<(int)id>(m_textures).loadFromFile(path)) {
	    throw std::runtime_error("Crash: missing resource " + path);
	}
	std::get<(int)id>(m_sprites).setTexture(std::get<(int)id>(m_textures));
    }
};

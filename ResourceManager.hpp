#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <array>

namespace RID {
    enum class Sprite {
	Tileset,
	OceanBkg,
	Cursor,
	Count
    };
    enum class Shader {
	Color,
	Count
    };
};

class ResourceManager {
public:
    ResourceManager();
    
    template <RID::Sprite id>
    sf::Sprite & GetSprite() {
	return std::get<static_cast<int>(id)>(m_sprites);
    }
    
private:
    std::array<sf::Sprite, static_cast<int>(RID::Sprite::Count)> m_sprites;
    std::array<sf::Texture, static_cast<int>(RID::Sprite::Count)> m_textures;
    std::array<sf::Shader, static_cast<int>(RID::Shader::Count)> m_shaders;

    template <RID::Sprite id>
    void SetupDrawable(const std::string & path) {
	if (!std::get<(int)id>(m_textures).loadFromFile(path)) {
	    throw std::runtime_error("Crash: missing resource " + path);
	}
	std::get<(int)id>(m_sprites).setTexture(std::get<(int)id>(m_textures));
    }
};

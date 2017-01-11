#pragma once

#include <SFML/Graphics.hpp>

class Cursor {
public:
    const sf::Vector2f & GetPosition() const;
    void SetPosition(const sf::Vector2f &);

private:
    sf::Vector2f m_position;
};

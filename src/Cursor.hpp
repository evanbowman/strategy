#pragma once

#include <SFML/Graphics.hpp>

class Camera;

class Cursor {
public:
    Cursor(const sf::RenderWindow &, const Camera &);
    const sf::Vector2f & GetPosition() const;
    void SetPosition(const sf::Vector2f &);

private:
    sf::Vector2f m_position;
};

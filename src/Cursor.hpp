#pragma once

#include <SFML/Graphics.hpp>

class Camera;
class Game;

class Cursor {
public:
    Cursor(const sf::RenderWindow &, const Camera &);
    const sf::Vector2f & GetPosition() const;
    void SetPosition(const sf::Vector2f &);
    void Update(Game & game);

private:
    sf::Vector2f m_position;
};

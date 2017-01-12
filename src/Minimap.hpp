#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <mutex>

class Game;

class Minimap : public sf::Drawable {
public:
    Minimap();
    void Update(Game &);
    bool Overlapping(const sf::Vector2f &);
    const sf::Vector2f & GetPosition() const;
    sf::Vector2f GetSize() const;
    
private:
    virtual void draw(sf::RenderTarget &, sf::RenderStates) const;

    mutable std::mutex m_mutex;
    sf::RectangleShape m_bkg;
    mutable sf::RectangleShape m_view;
};

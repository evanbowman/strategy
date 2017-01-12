#pragma once

#include <SFML/Graphics.hpp>

class Minimap : public sf::Drawable {
public:
    // ...
    
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const {
        target.draw(m_bkg, states);
	target.draw(m_view, states);
    }
    
    sf::RectangleShape m_bkg;
    sf::RectangleShape m_view;
};

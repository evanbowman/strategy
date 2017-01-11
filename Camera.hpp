#pragma once

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(const sf::Vector2f &);
    const sf::View & GetDrawableRegion() const;
    const sf::View & GetCameraRegion() const;
    
private:
    sf::View m_drawableRegion;
    sf::View m_cameraRegion;
};

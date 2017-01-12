#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "math.hpp"
#include "Cursor.hpp"

class Camera {
public:
    Camera(const sf::Vector2f &);
    const sf::View & GetDrawableRegion() const;
    const sf::View & GetCameraRegion() const;
    const sf::Vector2f & GetOffset() const;
    
    template <typename Game>
    void Update(Game & game, const sf::Time & elapsedTime) {
	const float speed = elapsedTime.asMicroseconds() * 0.0000025f;
	const sf::Vector2f & cursorPos =  game.GetCursor().GetPosition();
	const sf::Vector2f & cRSize = m_cameraRegion.getSize();
	const sf::Vector2f & cRCenter = m_cameraRegion.getCenter();
	if (!game.GetMinimap().Overlapping(game.GetCursor().GetPosition())) {
	    if (std::abs(cursorPos.y - (cRCenter.y + cRSize.y / 2)) < 48) {
		m_target = sf::Vector2f(m_target.x, m_target.y + speed * 100);
	    }
	    if (std::abs(cursorPos.y - (cRCenter.y - cRSize.y / 2)) < 48) {
		m_target = sf::Vector2f(m_target.x, m_target.y - speed * 100);
	    }
	    if (std::abs(cursorPos.x - (cRCenter.x + cRSize.x / 2)) < 48) {
		m_target = sf::Vector2f(m_target.x + speed * 100, m_target.y);
	    }
	    if (std::abs(cursorPos.x - (cRCenter.x - cRSize.x / 2)) < 48) {
		m_target = sf::Vector2f(m_target.x - speed * 100, m_target.y);
	    }
	}
	auto & defaultView = game.GetWindow().getDefaultView();
	const sf::Vector2f scaleFactors {
	    m_cameraRegion.getSize().x / defaultView.getSize().x,
	    m_cameraRegion.getSize().y / defaultView.getSize().y
	};
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	const float xStart = cRCenter.x - cRSize.x / 2.f;
	const float yStart = cRCenter.y - cRSize.y / 2.f;
	game.GetCursor().SetPosition({xStart + mousePos.x * scaleFactors.x,
				      yStart + mousePos.y * scaleFactors.y});
	m_cameraRegion.setCenter(math::lerp(m_target,
					    m_cameraRegion.getCenter(), speed));
    }
    
private:
    sf::View m_cameraRegion;
    sf::Vector2f m_target;
};

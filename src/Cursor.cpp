#include "Cursor.hpp"
#include "Camera.hpp"
#include "Game.hpp"

Cursor::Cursor(const sf::RenderWindow & window, const Camera & camera) {
    auto & defaultView = window.getDefaultView();
    auto & cameraView = camera.GetCameraRegion();
    const sf::Vector2f scaleFactors {
	cameraView.getSize().x / defaultView.getSize().x,
	cameraView.getSize().y / defaultView.getSize().y
    };
    sf::Vector2i cursorPos = sf::Mouse::getPosition();
    this->SetPosition({cursorPos.x * scaleFactors.x,
		       cursorPos.y * scaleFactors.y});
}

void Cursor::Update(Game & game) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	Minimap & minimap = game.GetMinimap();
	Camera & camera = game.GetCamera();
	HexGlobe<MapTile> & globe = game.GetGlobe();
	if (minimap.Overlapping(m_position)) {
	    const sf::Vector2f & mmPos = minimap.GetPosition();
	    sf::Vector2f mmSize = minimap.GetSize();
	    const sf::Vector2f mmFract {
		(m_position.x - mmPos.x) / mmSize.x,
		(m_position.y - mmPos.y) / mmSize.y
	    };
	    const sf::Vector2f target {
		mmFract.x * (globe.GetWidth() * 39.f),
		mmFract.y * (globe.GetHeight() * 36.f)
	    };
	    camera.Jump(target);
	}
    }
}

const sf::Vector2f & Cursor::GetPosition() const {
    return m_position;
}

void Cursor::SetPosition(const sf::Vector2f & position) {
    m_position = position;
}

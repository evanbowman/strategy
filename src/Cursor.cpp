#include "Cursor.hpp"
#include "Camera.hpp"

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

const sf::Vector2f & Cursor::GetPosition() const {
    return m_position;
}

void Cursor::SetPosition(const sf::Vector2f & position) {
    m_position = position;
}

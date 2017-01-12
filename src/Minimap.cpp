#include "Minimap.hpp"
#include "Game.hpp"

static const sf::Vector2f minimapBkgSize = {150.f, 96.f};

Minimap::Minimap() : m_bkg(minimapBkgSize) {
    m_bkg.setFillColor({22, 22, 59});
    m_bkg.setOutlineColor({255, 246, 243});
    m_bkg.setOutlineThickness(1.5f);
}

bool Minimap::Overlapping(const sf::Vector2f & point) {
    return (point.x >= m_bkg.getPosition().x &&
	    point.x <= m_bkg.getPosition().x + m_bkg.getSize().x &&
	    point.y >= m_bkg.getPosition().y &&
	    point.y <= m_bkg.getPosition().y + m_bkg.getSize().y);
}

void Minimap::Update(Game & game) {
    Camera & camera = game.GetCamera();
    const sf::Vector2f & cameraCenter = camera.GetCameraRegion().getCenter();
    const sf::Vector2f & viewSize = camera.GetCameraRegion().getSize();
    m_bkg.setPosition({
	    cameraCenter.x + viewSize.x / 2.f - minimapBkgSize.x - 4,
	    cameraCenter.y + viewSize.y / 2.f - minimapBkgSize.y - 4
	});
}

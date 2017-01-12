#include "Minimap.hpp"
#include "Game.hpp"

static const sf::Vector2f minimapBkgSize = {150.f, 96.f};

Minimap::Minimap() : m_bkg(minimapBkgSize) {
    m_bkg.setFillColor({22, 22, 59});
    m_bkg.setOutlineColor({255, 246, 243});
    m_bkg.setOutlineThickness(1.5f);
    m_view.setOutlineColor({255, 246, 243});
    m_view.setFillColor(sf::Color::Transparent);
    m_view.setOutlineThickness(1.5f);
}

const sf::Vector2f & Minimap::GetPosition() const {
    return m_bkg.getPosition();
}

sf::Vector2f Minimap::GetSize() const {
    return minimapBkgSize;
}

bool Minimap::Overlapping(const sf::Vector2f & point) {
    return (point.x >= m_bkg.getPosition().x &&
	    point.x <= m_bkg.getPosition().x + m_bkg.getSize().x &&
	    point.y >= m_bkg.getPosition().y &&
	    point.y <= m_bkg.getPosition().y + m_bkg.getSize().y);
}

void Minimap::Update(Game & game) {
    std::lock_guard<std::mutex> lk(m_mutex);
    Camera & camera = game.GetCamera();
    const sf::Vector2f & cameraCenter = camera.GetCameraRegion().getCenter();
    const sf::Vector2f & viewSize = camera.GetCameraRegion().getSize();
    m_bkg.setPosition({
	    cameraCenter.x + viewSize.x / 2.f - minimapBkgSize.x - 4,
	    cameraCenter.y + viewSize.y / 2.f - minimapBkgSize.y - 4
	});
    const int mapWidth = game.GetGlobe().GetWidth();
    const int mapHeight = game.GetGlobe().GetHeight();
    const sf::Vector2f fractWithinView({
	    viewSize.x / (mapWidth * 39.f),
	    viewSize.y / (mapHeight * 36.f)
	});
    m_view.setSize({
	    minimapBkgSize.x * fractWithinView.x,
	    minimapBkgSize.y * fractWithinView.y
	});
    m_view.setPosition({
	    m_bkg.getPosition().x + minimapBkgSize.x *
	    std::fmod(cameraCenter.x - viewSize.x / 2, mapWidth * 39.f) /
	    (mapWidth * 39.f),
	    m_bkg.getPosition().y + minimapBkgSize.y *
	    std::fmod(cameraCenter.y - viewSize.y / 2, mapHeight * 36.f) /
	    (mapHeight * 36.f)
	});
    if (m_view.getPosition().x < m_bkg.getPosition().x) {
	m_view.setPosition(m_view.getPosition().x + minimapBkgSize.x, m_view.getPosition().y);
    }
}

void Minimap::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    std::lock_guard<std::mutex> lk(m_mutex);
    target.draw(m_bkg, states);
    if (m_view.getPosition().x + m_view.getSize().x
	<= m_bkg.getPosition().x + m_bkg.getSize().x) {
	target.draw(m_view, states);
    } else {
	const sf::Vector2f viewSize = m_view.getSize();
	const float overhang =
	    (m_view.getPosition().x + m_view.getSize().x) -
	    (m_bkg.getPosition().x + m_bkg.getSize().x);
	m_view.setSize({viewSize.x - overhang, viewSize.y});
	target.draw(m_view, states);
	m_view.setSize({overhang, viewSize.y});
	m_view.setPosition(m_bkg.getPosition().x, m_view.getPosition().y);
	target.draw(m_view, states);
    }
}

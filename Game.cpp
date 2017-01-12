#include "Game.hpp"

// FIXME: Calculate these from aspect ratio rather than hard code
static const int drWidth = 784;
static const int drHeight = 490;

Game::Game() : m_globe(GenMap(66, 42)),
	       m_window(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen),
	       m_running(true),
	       m_camera({784, 490}),
	       m_cursor(m_window, m_camera) {
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true);
    m_window.setMouseCursorVisible(false);
    sf::Sprite & oceanSprite = m_resources.GetSprite<RID::Sprite::OceanBkg>();
    oceanSprite.setScale({(drWidth / 450.f) * 0.77f , (drHeight / 450.f) * 0.77f});
}

template <int margin = 0>
inline static bool IsWithinView(const sf::Vector2f & targetPos, const sf::View & view) {
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();
    if (targetPos.x > viewCenter.x - viewSize.x / 2 - margin &&
        targetPos.x < viewCenter.x + viewSize.x / 2 + margin &&
        targetPos.y > viewCenter.y - viewSize.y / 2 - margin &&
        targetPos.y < viewCenter.y + viewSize.y / 2 + margin) {
        return true;
    }
    return false;
}

void Game::DrawGraphics() {
    m_window.clear(sf::Color(128, 128, 128));
    m_window.setView(m_camera.GetCameraRegion());
    sf::Sprite & oceanSprite = m_resources.GetSprite<RID::Sprite::OceanBkg>();
    const sf::Vector2f & cameraViewCenter = m_camera.GetCameraRegion().getCenter();
    const sf::Vector2f & cameraViewSize = m_camera.GetCameraRegion().getSize();
    oceanSprite.setPosition({cameraViewCenter.x - cameraViewSize.x / 2.f,
			     cameraViewCenter.y - cameraViewSize.y / 2.f});
    m_window.draw(oceanSprite);
    using ZOrder = float;
    std::vector<std::pair<sf::Sprite, ZOrder>> drawables;
    m_globe.ForEach([this, &drawables](HexNode<MapTile> * node) {
	const HexCoord & coord = node->GetCoord();
	if (IsWithinView<96>({coord.col * 38.f, coord.row * 36.f},
			     m_camera.GetCameraRegion())) {
	    sf::Sprite & tileset = this->m_resources.GetSprite<RID::Sprite::Tileset>();
	    switch (node->data.type) {
	    case MapTile::Ocean: return;
	    case MapTile::Sand:
	    case MapTile::Steppe:
	    case MapTile::Forest:
	    case MapTile::Mountain:
		tileset.setTextureRect({node->data.type * 48, 0, 48, 56});
		// NOTE: Hex grid, so even and odd columns are offset a bit,
		//       hence the 'coord.col % 2'
		if (coord.col % 2) {
		    tileset.setPosition({39.f * coord.col, 36.f * coord.row});
		} else {
		    tileset.setPosition({39.f * coord.col, 36.f * coord.row + 18});
		}
		drawables.push_back({tileset, tileset.getPosition().y});
		break;
	    }
	}
    });
    std::sort(drawables.begin(), drawables.end(),
	      [](const std::pair<sf::Sprite, ZOrder> & lhs,
		 const std::pair<sf::Sprite, ZOrder> & rhs) {
		  return lhs.second < rhs.second;
	      });
    for (const auto & element : drawables) {
	m_window.draw(element.first);
    }
    sf::Sprite & cursorSprite = m_resources.GetSprite<RID::Sprite::Cursor>();
    cursorSprite.setPosition(m_cursor.GetPosition());
    m_window.draw(cursorSprite);
    m_window.display();
}

void Game::UpdateLogic() {
    const auto logicStart = std::chrono::high_resolution_clock::now();
    const sf::Time elapsedTime = m_logicClock.restart();
    m_camera.Update(*this, elapsedTime);
    m_globe.ForEach([](HexNode<MapTile> * node) {
	// Update logic...
    });
    const auto logicEnd = std::chrono::high_resolution_clock::now();
    const auto duration =
	std::chrono::duration_cast<std::chrono::nanoseconds>(logicEnd - logicStart);
    static const std::chrono::microseconds logicUpdateLimit(2000);
    std::this_thread::sleep_for(logicUpdateLimit - duration);
}

void Game::EventLoop() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
	switch (event.type) {
	case sf::Event::Closed:
	    m_window.close();
	    m_running = false;
	    break;

	}
    }
}

bool Game::IsRunning() const {
    return m_running;
}

Cursor & Game::GetCursor() {
    return m_cursor;
}

const sf::RenderWindow & Game::GetWindow() const {
    return m_window;
}

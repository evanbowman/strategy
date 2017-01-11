#include "Game.hpp"

// FIXME: Calculate these from aspect ratio rather than hard code
static const int drWidth = 784;
static const int drHeight = 490;

Game::Game() : m_globe(GenMap(66, 42)),
	       m_window(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen),
	       m_running(true),
	       m_camera({784, 490}) {
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true);
    m_window.setMouseCursorVisible(false);
    sf::Sprite & oceanSprite = m_resources.GetSprite<RID::OceanBkg>();
    oceanSprite.setScale({(drWidth / 450.f) * 0.77f , (drHeight / 450.f) * 0.77f});
    m_target.create(drWidth, drHeight);
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
    m_target.clear(sf::Color::Transparent);
    m_window.setView(m_camera.GetCameraRegion());
    m_target.setView(m_camera.GetDrawableRegion());
    sf::Sprite & oceanSprite = m_resources.GetSprite<RID::OceanBkg>();
    m_window.draw(oceanSprite);
    using ZOrder = float;
    std::vector<std::pair<sf::Sprite, ZOrder>> drawables;
    m_globe.ForEach([this, &drawables](HexNode<MapTile> * node) {
	const HexCoord & coord = node->GetCoord();
	if (IsWithinView<48>({coord.col * 38.f, coord.row * 36.f},
			     m_camera.GetCameraRegion())) {
	    sf::Sprite & tileset = this->m_resources.GetSprite<RID::Tileset>();
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
	m_target.draw(element.first);
    }
    sf::Sprite & cursorSprite = m_resources.GetSprite<RID::Cursor>();
    cursorSprite.setPosition(m_cursor.GetPosition());
    m_target.draw(cursorSprite);
    m_target.display();
    m_window.draw(sf::Sprite(m_target.getTexture()));
    m_window.display();
}

void Game::UpdateLogic() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void Game::EventLoop() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
	switch (event.type) {
	case sf::Event::Closed:
	    m_window.close();
	    m_running = false;
	    break;

	case sf::Event::MouseMoved:
	    {
		auto & defaultView = m_window.getDefaultView();
		auto & cameraView = m_camera.GetCameraRegion();
		const sf::Vector2f scaleFactors {
		    cameraView.getSize().x / defaultView.getSize().x,
		    cameraView.getSize().y / defaultView.getSize().y
		};
		m_cursor.SetPosition({event.mouseMove.x * scaleFactors.x,
				      event.mouseMove.y * scaleFactors.y});
	    }
	    break;
	}
    }
}

bool Game::IsRunning() const {
    return m_running;
}

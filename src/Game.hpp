#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <chrono>
#include "GenMap.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "Cursor.hpp"
#include "Minimap.hpp"

class Game {
public:
    Game();
    void DrawGraphics();
    void UpdateLogic();
    void EventLoop();
    bool IsRunning() const;
    Cursor & GetCursor();
    Camera & GetCamera();
    Minimap & GetMinimap();
    HexGlobe<MapTile> & GetGlobe();
    const sf::RenderWindow & GetWindow() const;
    
private:
    HexGlobe<MapTile> m_globe;
    sf::Clock m_logicClock;
    sf::RenderWindow m_window;
    ResourceManager m_resources;
    bool m_running;
    Camera m_camera;
    Cursor m_cursor;
    Minimap m_minimap;
};

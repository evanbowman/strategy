#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "GenMap.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "Cursor.hpp"

class Game {
public:
    Game();
    void DrawGraphics();
    void UpdateLogic();
    void EventLoop();
    bool IsRunning() const;
    
private:
    HexGlobe<MapTile> m_globe;
    sf::RenderWindow m_window;
    sf::RenderTexture m_target;
    ResourceManager m_resources;
    bool m_running;
    Camera m_camera;
    Cursor m_cursor;
};

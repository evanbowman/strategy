#include "Camera.hpp"

Camera::Camera(const sf::Vector2f & drawableArea) :
    m_drawableRegion({0, 0, drawableArea.x, drawableArea.y}),
    m_cameraRegion({0, 0, drawableArea.x * 0.77f, drawableArea.y * 0.77f}) {}

const sf::View & Camera::GetDrawableRegion() const {
    return m_drawableRegion;
}

const sf::View & Camera::GetCameraRegion() const {
    return m_cameraRegion;
}


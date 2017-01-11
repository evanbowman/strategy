#include "Cursor.hpp"

const sf::Vector2f & Cursor::GetPosition() const {
    return m_position;
}

void Cursor::SetPosition(const sf::Vector2f & position) {
    m_position = position;
}

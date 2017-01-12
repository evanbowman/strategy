#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
    this->SetupDrawable<RID::Sprite::OceanBkg>("../res/OceanBkg.png");
    this->SetupDrawable<RID::Sprite::Tileset>("../res/Tileset.png");
    this->SetupDrawable<RID::Sprite::Cursor>("../res/Cursor.png");
    // ...
}

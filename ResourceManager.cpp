#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
    this->SetupDrawable<RID::OceanBkg>("res/OceanBkg.png");
    this->SetupDrawable<RID::Tileset>("res/Tileset.png");
    this->SetupDrawable<RID::Cursor>("res/Cursor.png");
    // ...
}

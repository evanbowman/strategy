#pragma once

#include "HexGlobe.hpp"
#include "MapTile.hpp"
#include "stdlib.h"

HexGlobe<MapTile> GenMap(const size_t w, const size_t h);

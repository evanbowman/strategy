#include "GenMap.hpp"

HexGlobe<MapTile> GenMap(const size_t w, const size_t h) {
    HexGlobe<MapTile> globe(w, h);
    globe.ForEach([](HexNode<MapTile> * node) {
	node->data.type = static_cast<MapTile::Type>(rand() % 2);
    });
    const auto ApplySmoothing = [](HexNode<MapTile> * node) {
	uint8_t count = 0;
	for (auto neighbor : node->GetNeighbors()) {
	    if (neighbor) {
		if (neighbor->data.type == MapTile::Type::Sand) {
		    ++count;
		}
	    }
	}
	if (node->data.type == MapTile::Type::Sand) {
	    if (count < 2) {
		node->data.type = MapTile::Type::Ocean;
	    } else {
		node->data.type = MapTile::Type::Sand;
	    }
	} else {
	    if (count > 4) {
		node->data.type = MapTile::Type::Sand;
	    } else {
		node->data.type = MapTile::Type::Ocean;
	    }
	}
    };
    for (int i = 0; i < 5; ++i) {
	globe.ForEach(ApplySmoothing);
    }
    globe.ForEach([](HexNode<MapTile> * node) {
	if (node->data.type == MapTile::Type::Sand) {
	    uint8_t count = 0;
	    for (auto neighbor : node->GetNeighbors()) {
		if (neighbor) {
		    if (neighbor->data.type == MapTile::Ocean) {
			++count;
		    }
		}
	    }
	    if (count == 0) {
		node->data.type = MapTile::Type::Steppe;
	    }
	}
    });
    return globe;
}

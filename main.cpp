#include <iostream>
#include "GenMap.hpp"

#include <chrono>

int main() {
    srand(time(nullptr));
    auto globe = GenMap(80, 52);
    int map[52][80] = {};
    globe.ForEach([&map](HexNode<MapTile> * node) {
        auto coord = node->GetCoord();
	map[coord.row][coord.col] = node->data.type;
    });
    for (int row = 0; row < 52; ++row) {
	for (int col = 0; col < 80; ++col) {
	    switch (map[row][col]) {
	    case MapTile::Ocean:
		std::cout << "\x1B[34m" << '~';
		break;

	    case MapTile::Sand:
		std::cout << "\x1B[33m" << '*';
		break;

	    case MapTile::Steppe:
		std::cout << "\x1B[0m" << "s";
		break;
		
	    case MapTile::Forest:
		std::cout << "\x1B[32m" << "t";
		break;

	    case MapTile::Mountain:
		std::cout << "\x1B[37m" << "A";
		break;

	    default:
		abort();
		break;
	    }
	}
	std::cout << "\n" << "\x1B[0m";
    }
}

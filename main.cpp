#include <iostream>
#include "GenMap.hpp"

#include <chrono>

int main() {
    srand(time(nullptr));
    auto globe = GenMap(90, 32);
    int map[32][90] = {};
    globe.ForEach([&map](HexNode<MapTile> * node) {
        auto coord = node->GetCoord();
	map[coord.row][coord.col] = node->data.type;
    });
    for (int row = 0; row < 32; ++row) {
	for (int col = 0; col < 90; ++col) {
	    switch (map[row][col]) {
	    case MapTile::Ocean:
		std::cout << "\x1B[34m" << '~';
		break;

	    case MapTile::Sand:
		std::cout << "\x1B[37m" << '*';
		break;

	    case MapTile::Steppe:
		std::cout << "\x1B[32m" << "s";
		break;
	    }
	}
	std::cout << "\n" << "\x1B[0m";
    }
}

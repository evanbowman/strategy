#include "GenMap.hpp"

HexGlobe<MapTile> GenMap(const size_t w, const size_t h) {
    HexGlobe<MapTile> globe(w, h);
    globe.ForEach([](HexNode<MapTile> * node) {
	node->data.type = static_cast<MapTile::Type>(rand() % 7 < 3);
    });
    const auto ApplySmoothing = [](HexNode<MapTile> * node) {
	uint8_t count = 0;
	for (auto neighbor : node->GetNeighbors()) {
	    if (neighbor) {
		if (neighbor->data.type == MapTile::Sand) {
		    ++count;
		}
	    }
	}
	if (node->data.type == MapTile::Sand) {
	    if (count < 1) {
		node->data.type = MapTile::Ocean;
	    } else {
		node->data.type = MapTile::Sand;
	    }
	} else {
	    if (count > 3) {
		node->data.type = MapTile::Sand;
	    } else {
		node->data.type = MapTile::Ocean;
	    }
	}
    };
    for (int i = 0; i < 2; ++i) {
	globe.ForEach(ApplySmoothing);
    }
    globe.ForEach([](HexNode<MapTile> * node) {
	if (node->data.type == MapTile::Sand) {
	    for (auto neighbor : node->GetNeighbors()) {
		if (neighbor) {
		    if (neighbor->data.type == MapTile::Ocean) {
			return;
		    }
		}
	    }
	    std::stack<HexNode<MapTile> *> stack;
	    std::unordered_map<HexNode<MapTile> *, int> distance;
	    std::set<int> results;
	    distance[node] = 0;
	    stack.push(node);
	    while (!stack.empty()) {
		auto current = stack.top();
		stack.pop();
		for (auto neighbor : current->GetNeighbors()) {
		    if (neighbor) {
			if (distance.find(neighbor) == distance.end()) {
			    if (neighbor->data.type == MapTile::Ocean) {
				results.insert(distance[current] + 1);
				continue;
			    }
			    distance[neighbor] = distance[current] + 1;
			    stack.push(neighbor);
			}
		    }
		}
	    }
	    node->data.type =
		static_cast<MapTile::Type>(std::min((int)MapTile::Forest, *results.begin()));
	}
    });
    return globe;
}

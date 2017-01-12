#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>

struct HexCoord {
    long col, row;
};

template <typename T>
struct HexNode {
    HexNode() : neighbors{} {}

    HexNode(const int col, const int row) : neighbors{},
					    coord({col, row}) {}
    
    T data;
	
    std::array<HexNode<T> *, 6> & GetNeighbors() {
	return neighbors;
    }

    const HexCoord & GetCoord() const {
	return coord;
    }

    void SetCoord(const HexCoord & coord) {
        this->coord = coord;
    }
    
    template <typename _>
    friend class HexGlobe;
	
private:    
    std::array<HexNode<T> *, 6> neighbors;
    HexCoord coord;
};

template <typename T>
class HexGlobe {
public:	
    template <typename F>
    std::vector<HexNode<T> *> Filter(F && expr) {
	std::vector<HexNode<T> *> matches;
	this->ForEach([&expr, &matches](HexNode<T> * node) {
	    if (expr(node)) {
		matches.push_back(node);
	    }
	});
	return matches;
    }
	
    template <typename F>
    void ForEach(F && cb) {
	for (auto & node : m_nodes) {
	    cb(node);
	}
    }

    void Create(const int w, const int h) {
	assert(w % 2 == 0);
	m_width = w;
	m_height = h;
	m_nodes.clear();
	m_nodes.resize(w * h);
	size_t slotsUsed = 0;
	std::vector<std::vector<HexNode<T> *>> grid(w);
	for (int col = 0; col < w; ++col) {
	    grid[col].reserve(h);
	    for (int row = 0; row < h; ++row) {
	        grid[col].push_back(&m_nodes[slotsUsed++]);
		grid[col].back()->SetCoord({col, row});
	    }
	}
	enum Edge {
	    Top,
	    TopRight,
	    BottomRight,
	    Bottom,
	    BottomLeft,
	    TopLeft
	};
	// Loop below creates a hex graph that wraps horizontally
	for (size_t col = 0; col < w; ++col) {
	    for (size_t row = 0; row < h; ++row) {
		const auto MapNodeToHexEdge =
		    [&grid, col, row](const int edge,
				      const size_t targetCol,
				      const size_t targetRow) {
			grid[col][row]->neighbors[edge] = grid[targetCol][targetRow];
		    };
		if (row == 0) {
		    MapNodeToHexEdge(Bottom, col, row + 1);
		} else if (row == h - 1) {
		    MapNodeToHexEdge(Top, col, row - 1);
		} else {
		    MapNodeToHexEdge(Top, col, row - 1);
		    MapNodeToHexEdge(Bottom, col, row + 1);
		}
		if (col == 0) {
		    MapNodeToHexEdge(TopRight, col + 1, row);
		    MapNodeToHexEdge(TopLeft, w - 1, row);
		    if (row == h - 1) {
			MapNodeToHexEdge(BottomRight, col + 1, 0);
			MapNodeToHexEdge(BottomLeft, w - 1, 0);
		    } else {
			MapNodeToHexEdge(BottomRight, col + 1, row + 1);
			MapNodeToHexEdge(BottomLeft, w - 1, row + 1);
		    }
		} else if (col == w - 1) {
		    MapNodeToHexEdge(BottomLeft, col - 1, row);
		    MapNodeToHexEdge(BottomRight, w - 1, row);
		    if (row == 0) {
			MapNodeToHexEdge(TopLeft, col - 1, h - 1);
			MapNodeToHexEdge(TopRight, 0, h - 1);
		    } else {
			MapNodeToHexEdge(TopLeft, col - 1, row - 1);
			MapNodeToHexEdge(TopRight, 0, row - 1);
		    }
		} else {
		    const bool isEven = col % 2;
		    if (isEven) {
			MapNodeToHexEdge(BottomLeft, col - 1, row);
			MapNodeToHexEdge(BottomRight, col + 1, row);
			if (row == 0) {
			    MapNodeToHexEdge(TopLeft, col - 1, h - 1);
			    MapNodeToHexEdge(TopRight, col + 1, h - 1);
			} else {
			    MapNodeToHexEdge(TopLeft, col - 1, row - 1);
			    MapNodeToHexEdge(TopRight, col + 1, row - 1);
			}
		    } else {
			MapNodeToHexEdge(TopRight, col + 1, row);
			MapNodeToHexEdge(TopLeft, col - 1, row);
			if (row == h - 1) {
			    MapNodeToHexEdge(BottomRight, col + 1, 0);
			    MapNodeToHexEdge(BottomLeft, col - 1, 0);
			} else {
			    MapNodeToHexEdge(BottomRight, col + 1, row + 1);
			    MapNodeToHexEdge(BottomLeft, col - 1, row + 1);
			}
		    }
		}
	    }
	}
    }

    int GetWidth() const {
	return m_width;
    }

    int GetHeight() const {
	return m_height;
    }
    
private:
    std::vector<HexNode<T>> m_nodes;
    int m_width;
    int m_height;
};

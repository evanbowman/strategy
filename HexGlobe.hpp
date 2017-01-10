#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <tuple>
#include <cassert>

struct HexCoord {
    int col, row;
};

template <typename T>
struct HexNode {
    HexNode() : neighbors{}, colored(false) {}

    HexNode(const int col, const int row) : neighbors{},
					    colored(false),
					    coord({col, row}) {}
    
    T data;
	
    std::array<HexNode<T> *, 6> & GetNeighbors() {
	return neighbors;
    }

    const HexCoord & GetCoord() const {
	return coord;
    }
	
    template <typename _>
    friend class HexGlobe;
	
private:    
    void SetCoord(const HexCoord & coord) {
        this->coord = coord;
    }

    std::array<HexNode<T> *, 6> neighbors;
    HexCoord coord;
    bool colored;
};

template <typename T>
class HexGlobe {
public:
    HexGlobe(HexGlobe<T> && other) : m_entryPt(other.m_entryPt) {
	other.m_entryPt = nullptr;
    }
	
    const HexGlobe<T> & operator=(HexGlobe<T> && other) {
	m_entryPt = other.m_entryPt;
	other.m_entryPt = nullptr;
	return *this;
    }
	
    HexGlobe(const HexGlobe<T> & other) = delete;
	
    const HexGlobe<T> & operator=(const HexGlobe<T> & other) = delete;

    ~HexGlobe() {
	// TODO: free graph... very carefully...
    }

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
	std::stack<HexNode<T> *> stack;
	stack.push(m_entryPt);
	while (!stack.empty()) {
	    auto current = stack.top();
	    if (current->colored) {
		stack.pop();
		continue;
	    }
	    current->colored = true;
	    stack.pop();
	    cb(current);
	    for (auto node : current->neighbors) {
		if (node) {
		    if (!node->colored) {
			stack.push(node);
		    }
		}
	    }
	}
	stack.push(m_entryPt);
	while (!stack.empty()) {
	    auto current = stack.top();
	    current->colored = false;
	    stack.pop();
	    for (auto node : current->neighbors) {
		if (node) {
		    if (node->colored) {
			stack.push(node);
		    }	
		}
	    }
	}
    }

    explicit HexGlobe(const size_t w, const size_t h) {
	assert(w % 2 == 0);
	std::vector<std::vector<HexNode<T> *>> grid(w);
	for (size_t col = 0; col < w; ++col) {
	    grid[col].reserve(h);
	    for (size_t row = 0; row < h; ++row) {
	        grid[col].push_back(new HexNode<T>(col, row));
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
		if (row == 0) {
		    grid[col][row]->neighbors[Bottom] = grid[col][row + 1];
		} else if (row == h - 1) {
		    grid[col][row]->neighbors[Top] = grid[col][row - 1];
		} else {
		    grid[col][row]->neighbors[Top] = grid[col][row - 1];
		    grid[col][row]->neighbors[Bottom] = grid[col][row + 1];
		}
		if (col == 0) {
		    grid[col][row]->neighbors[TopRight] = grid[col + 1][row];
		    grid[col][row]->neighbors[TopLeft] = grid[w - 1][row];
		    if (row == h - 1) {
			grid[col][row]->neighbors[BottomRight] = grid[col + 1][0];
			grid[col][row]->neighbors[BottomLeft] = grid[w - 1][0];
		    } else {
			grid[col][row]->neighbors[BottomRight] = grid[col + 1][row + 1];
			grid[col][row]->neighbors[BottomLeft] = grid[w - 1][row + 1];
		    }
		} else if (col == w - 1) {
		    grid[col][row]->neighbors[BottomLeft] = grid[col - 1][row];
		    grid[col][row]->neighbors[BottomRight] = grid[w - 1][row];
		    if (row == 0) {
			grid[col][row]->neighbors[TopLeft] = grid[col - 1][h - 1];
			grid[col][row]->neighbors[TopRight] = grid[0][h - 1];
		    } else {
			grid[col][row]->neighbors[TopLeft] = grid[col - 1][row - 1];
			grid[col][row]->neighbors[TopRight] = grid[0][row - 1];
		    }
		} else {
		    const bool isEven = col % 2;
		    if (isEven) {
			grid[col][row]->neighbors[BottomLeft] = grid[col - 1][row];
			grid[col][row]->neighbors[BottomRight] = grid[col + 1][row];
			if (row == 0) {
			    grid[col][row]->neighbors[TopLeft] = grid[col - 1][h - 1];
			    grid[col][row]->neighbors[TopRight] = grid[col + 1][h - 1];
			} else {
			    grid[col][row]->neighbors[TopLeft] = grid[col - 1][row - 1];
			    grid[col][row]->neighbors[TopRight] = grid[col + 1][row - 1];
			}
		    } else {
			grid[col][row]->neighbors[TopRight] = grid[col + 1][row];
			grid[col][row]->neighbors[TopLeft] = grid[col - 1][row];
			if (row == h - 1) {
			    grid[col][row]->neighbors[BottomRight] = grid[col + 1][0];
			    grid[col][row]->neighbors[BottomLeft] = grid[col - 1][0];
			} else {
			    grid[col][row]->neighbors[BottomRight] = grid[col + 1][row + 1];
			    grid[col][row]->neighbors[BottomLeft] = grid[col - 1][row + 1];
			}
		    }
		}
	    }
	}
	m_entryPt = grid[0][0];
    }
    
private:
    HexNode<T> * m_entryPt;
};

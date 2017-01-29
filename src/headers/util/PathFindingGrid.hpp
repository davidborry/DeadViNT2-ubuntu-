#ifndef PATHFINDINGGRID_HPP
#define PATHFINDINGGRID_HPP

#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include "SFML/System/Vector2.hpp"



class Node{

public:

	Node();
	Node(int x, int y);
	bool isSolid() const;
	void setSolid(bool solid);

	void addNeighbour(Node* neighbour);
	void print();

	int getX() const;
	int getY() const;

	std::vector<Node*> getNeighbours();

private:
	std::vector<Node*> mNeighbours;
	bool mSolid = false;

	int x;
	int y;
};


class PathFindingGrid{

public:

	struct Position{
		int x;
		int y;

	};

	PathFindingGrid(int lines, int cols);

	std::vector<sf::Vector2f> findPath(Position start, Position end);

	std::unordered_map<Node*,Node*> searchPath(Position start, Position end);
	std::vector<sf::Vector2f> getPath(Position start, Position end, std::unordered_map<Node*, Node*>& cameFrom);
	void addNeighbours();
	bool isSolid(int x, int y) const;
	void setSolid(int x, int y, bool solid);
	void print();

	Node* getNode(int x, int y);

	void adjustNodePosition(Position& p);

private:
	int mLines;
	int mCols;

	Node** mNodes;
};

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};


typedef std::vector<sf::Vector2f> Path;

#endif
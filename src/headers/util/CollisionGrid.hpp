#ifndef COLLISIONGRID_HPP
#define COLLISIONGRID_HPP

#include "CollisionCell.hpp"
#include <array>

class CollisionGrid{
	
public:
	CollisionGrid(float worldWidth, float worldHeight, float cellWidth = 100.f, float cellHeight = 100.f);

	void build(std::vector<SceneNode*> nodes);
	void clear();

	void checkCollisions(std::set<SceneNode::Pair>& collisionsPair);


	void print();

private:
	int mLines;
	int mCols;
	float mCellWidth;
	float mCellHeight;

	CollisionCell **mCells;

};

#endif
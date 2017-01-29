#include "../../headers/util/CollisionGrid.hpp"

CollisionGrid::CollisionGrid(float worldWidth, float worldHeight, float cellWidth, float cellHeight) :
mCellHeight(cellHeight),
mCellWidth(cellWidth),
mLines(std::ceil(worldWidth/cellWidth)),
mCols(std::ceil(worldHeight/cellHeight))

{
	printf("%i,%i \n", mLines, mCols);


}

void CollisionGrid::build(std::vector<SceneNode*> nodes){
	mCells = new CollisionCell*[mLines];

	for (int i = 0; i < mLines; i++)
		mCells[i] = new CollisionCell[mCols];
	
	for (int i = 0; i < nodes.size(); i++){
		/*sf::Vector2f pos = nodes[i]->getWorldPosition();
		int x = floor(pos.x / mCellWidth);
		int y = floor(pos.y / mCellHeight);

		if (x < mLines && x >= 0 && y < mCols && y >= 0)
			mCells[x][y].addNode(nodes[i]);*/

		sf::Vector2f topLeft(nodes[i]->getBoundingRect().left, nodes[i]->getBoundingRect().top);
		sf::Vector2f topRight = topLeft + sf::Vector2f(nodes[i]->getBoundingRect().width, 0.f);
		sf::Vector2f downLeft = topLeft + sf::Vector2f(0.f, nodes[i]->getBoundingRect().height);
		sf::Vector2f downRight = topLeft + sf::Vector2f(nodes[i]->getBoundingRect().width, nodes[i]->getBoundingRect().height);

		sf::Vector2f angles[4] = {topLeft,topRight,downLeft,downRight};

		for (int k = 0; k < 4; k++){

			sf::Vector2f pos = angles[k];
			int x = floor(pos.x / mCellWidth);
			int y = floor(pos.y / mCellHeight);

			if (x < mLines && x >= 0 && y < mCols && y >= 0)
				mCells[x][y].addNode(nodes[i]);
		}
		/*printf("%f,%f - ", topLeft.x, topLeft.y);
		printf("%f,%f - ", topRight.x, topRight.y);
		printf("%f,%f - ", downLeft.x, downLeft.y);
		printf("%f,%f\n", downRight.x, downRight.y);*/

	}
	
}

void CollisionGrid::checkCollisions(std::set<SceneNode::Pair>& collisionSet){
	for (int i = 0; i < mLines; i++)
		for (int j = 0; j < mCols; j++)
			mCells[i][j].checkCollisions(collisionSet);
}

void CollisionGrid::print(){
	printf("%i\n", mCells[0][0].getNodes().size());
	}


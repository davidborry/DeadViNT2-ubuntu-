#include "../../headers/util/PathFindingGrid.hpp"
#include "../../headers/util/Utility.hpp"

namespace std
{
	//namespace tr1
	//{
	// Specializations for unordered containers

	template <>
	struct hash<Node> : public unary_function<Node, size_t>
	{
		size_t operator()(const Node& value) const
		{
			return 0;
		}
	};

	//} // namespace tr1

	template <>
	struct equal_to<Node> : public unary_function<Node, bool>
	{
		bool operator()(const Node& x, const Node& y) const
		{
			return false;
		}
	};

}


Node::Node(int x, int y):
x(x),
y(y)
{
}

Node::Node():
x(0),
y(0)
{}

bool Node::isSolid() const{
	return mSolid;
}

void Node::setSolid(bool solid){
	mSolid = solid;
}

void Node::addNeighbour(Node* neighbour){
	//printf("%i", neighbour->isSolid());
	mNeighbours.push_back(neighbour);
}

int Node::getX() const{
	return x;
}

int Node::getY() const{
	return y;
}

void Node::print(){
	if (isSolid())
		printf("X ");
	else
		printf("_ ");

//	printf("%i ", mNeighbours.size());

//	printf("%i,%i ", x, y);
}

std::vector<Node*> Node::getNeighbours(){
	return mNeighbours;
}

PathFindingGrid::PathFindingGrid(int lines, int cols):
mLines(lines),
mCols(cols)
{
	mNodes = new Node*[mLines];

	for (int i = 0; i < mLines; i++){
		mNodes[i] = new Node[mCols];
		for (int j = 0; j < mCols; j++)
			mNodes[i][j] = Node(i, j);
	}

	addNeighbours();
}

void PathFindingGrid::setSolid(int x, int y, bool solid){
	//printf("%i,%i\n", mLines, mCols);
	mNodes[y][x].setSolid(solid);
}

void PathFindingGrid::print(){
	for (int i = 0; i < mLines; i++){
		for (int j = 0; j < mCols; j++)
			mNodes[i][j].print();
		printf("\n");
	}
	
}

void PathFindingGrid::addNeighbours(){
	for (int i = 0; i < mLines; i++)
		for (int j = 0; j < mCols; j++){

			if (i - 1 >= 0)
				mNodes[i][j].addNeighbour(&mNodes[i - 1][j]);

			if (i + 1 < mLines)
				mNodes[i][j].addNeighbour(&mNodes[i + 1][j]);

			if (j - 1 >= 0)
				mNodes[i][j].addNeighbour(&mNodes[i][j - 1]);

			if (j + 1 < mCols)
				mNodes[i][j].addNeighbour(&mNodes[i][j + 1]);

			if (j + 1 < mCols && i - 1 >= 0)
				mNodes[i][j].addNeighbour(&mNodes[i - 1][j + 1]);

			if (j + 1 < mCols && i + 1 < mLines)
				mNodes[i][j].addNeighbour(&mNodes[i + 1][j + 1]);

			if (j - 1 >= 0 && i - 1 >= 0)
				mNodes[i][j].addNeighbour(&mNodes[i - 1][j - 1]);

			if (j - 1 >= 0 && i + 1 < mLines)
				mNodes[i][j].addNeighbour(&mNodes[i + 1][j - 1]);

			
		}
}

Node* PathFindingGrid::getNode(int x, int y){
	return &mNodes[x][y];
}


std::unordered_map<Node*,Node*> PathFindingGrid::searchPath(PathFindingGrid::Position start, PathFindingGrid::Position end){

	std::unordered_map<Node*, Node*> cameFrom;
	std::unordered_map<Node*, int> costSoFar;

	PriorityQueue<Node*, double> frontier;
	frontier.put(&mNodes[start.y][start.x],0);

	Node* startNode = &mNodes[start.y][start.x];
	cameFrom[startNode] = startNode;
	costSoFar[startNode] = 0;

	while (!frontier.empty()){
		Node* current = frontier.get();

		if (current->getX() == end.y && current->getY() == end.x)
			break;
		
		for (Node* next : current->getNeighbours()){
			int newCost = costSoFar[current] + 1;

			if (!next->isSolid() && ( !costSoFar.count(next) || newCost < costSoFar[next])){
				costSoFar[next] = newCost;
				int priority = newCost + std::abs(next->getX() - end.y) + std::abs(next->getY() - end.x);
			//	printf("%i,%i | %i,%i\n", next->getX(), next->getY(), end.y, end.x);
				//printf("%d\n", priority);
				frontier.put(next, priority);
				cameFrom[next] = current;
				
			}
		}
	}

	return cameFrom;
}


/**
Return found path in world coordinates
*/
std::vector<sf::Vector2f> PathFindingGrid::getPath(Position start, Position end, std::unordered_map<Node*, Node*>& cameFrom){
	std::vector<sf::Vector2f> path;

	Node* current = &mNodes[end.y][end.x];
	Node* startNode = &mNodes[start.y][start.x];

	
		path.push_back(sf::Vector2f(current->getY()*100 + 50, current->getX()*100 +50));
	while (current != startNode) {
		current = cameFrom[current];
		if (current == NULL)
			break;
		path.push_back(sf::Vector2f(current->getY()*100 + 50, current->getX()*100 + 50 ));
	}

	//path.push_back({start.x*100,start.y*100}); // optional
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<sf::Vector2f> PathFindingGrid::findPath(Position start, Position end){
	adjustNodePosition(end);
	adjustNodePosition(start);

	std::unordered_map<Node*, Node*> map = searchPath(start, end);
	return getPath(start, end, map);
}

void PathFindingGrid::adjustNodePosition(PathFindingGrid::Position& p){
	while (p.x < 0)
		p.x++;

	while (p.y < 0)
		p.y++;

	while (p.x >= mLines)
		p.x--;

	while (p.y >= mCols)
		p.y--;
}
 
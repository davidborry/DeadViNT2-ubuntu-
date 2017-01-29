#ifndef COLLISIONCELL_HPP
#define COLLISIONCELL_HPP

#include "../scene/SceneNode.hpp"

class CollisionCell{
	
public :
	explicit CollisionCell();

	void checkCollisions(std::set<SceneNode::Pair>& collisionsPair);
	void addNode(SceneNode* node);
	std::vector<SceneNode*> getNodes();
	void clear();

private:
	std::vector<SceneNode*> mNodes;
};

#endif
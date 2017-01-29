#include "../../headers/util/CollisionCell.hpp"

CollisionCell::CollisionCell() :
mNodes()
{

}

void CollisionCell::checkCollisions(std::set<SceneNode::Pair>& collisionsPair){
	for (int i = 0; i < mNodes.size(); i++)
		for (int j = i+1; j < mNodes.size(); j++)
			if (collision(*mNodes[i], *mNodes[j]) && !mNodes[i]->isDestroyed() && !mNodes[j]->isDestroyed())
				collisionsPair.insert(std::minmax(mNodes[i], mNodes[j]));
		
}


void CollisionCell::addNode(SceneNode* node){
	for (int i = 0; i < mNodes.size(); i++)
		if (mNodes[i] == node)
			return;
	mNodes.push_back(node);
}

std::vector<SceneNode*> CollisionCell::getNodes(){
	return mNodes;
}

void CollisionCell::clear(){
	mNodes.clear();
}
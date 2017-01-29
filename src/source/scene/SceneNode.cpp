#include "../../headers/scene/SceneNode.hpp"
#include "../../headers/commands/Command.hpp"
#include "../../headers/util/Utility.hpp"
#include "../../headers/util/foreach.hpp"

SceneNode::SceneNode(Category::Type category) : 
mDefaultCategory(category),
mParent(nullptr)
{
}

void SceneNode::attachChild(Ptr child){
	//printf("node");
	child->mParent = this;
	//getRoot()->addNode(child.get());
	
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node){

	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr & p) -> bool {
		return p.get() == &node;
	});

	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;

	mChildren.erase(found);

	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const{

	//drawBoundingRect(target,states);
	states.transform *= getTransform();
	drawCurrent(target, states);
	drawChildren(target, states);
	
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const{
	for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr){
		(*itr)->draw(target, states);
	}
}

void SceneNode::update(sf::Time dt, CommandQueue& commands){
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands){}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands){
	for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr){
		(*itr)->update(dt, commands);
	}
}

sf::Transform SceneNode::getWorldTransform() const{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform()*transform;

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const{
	return getWorldTransform() * sf::Vector2f();
}

unsigned int SceneNode::getCategory() const{
	return mDefaultCategory;
}

void SceneNode::onCommand(const Command& command, sf::Time dt){
	if (command.category & getCategory())
		command.action(*this, dt);

	for (auto child = mChildren.begin(); child != mChildren.end(); ++child)
		(*child)->onCommand(command, dt);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;

	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

sf::FloatRect SceneNode::getBoundingRect() const{
	return sf::FloatRect();
}

bool SceneNode::isDestroyed() const{
	return false;
	//No need for removal by default
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<SceneNode::Pair>& collisionPairs){
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	FOREACH(Ptr& child, mChildren){
		child->checkNodeCollision(node, collisionPairs);
	}
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<SceneNode::Pair>& collisionPairs){
	checkNodeCollision(sceneGraph, collisionPairs);

	FOREACH(Ptr& child, sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

float distance(const SceneNode& lhs, const SceneNode& rhs){
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

bool collision(const SceneNode& lhs, const SceneNode& rhs){
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2){

	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2)
		return true;

	else if (type1 & category2 && type2 & category1){
		std::swap(colliders.first, colliders.second);
		return true;
	}

	else
		return false;
}

bool SceneNode::isMarkedForRemoval() const {
	return isDestroyed();
}

void SceneNode::removeWrecks(){

	//First remove wrecks from the total nodes vector
	/*auto wreckfieldBegin1 = std::remove_if(mNodes.begin(), mNodes.end(),
		std::mem_fn(&SceneNode::isMarkedForRemoval));

	mNodes.erase(wreckfieldBegin1, mNodes.end());*/

	//Then clear the SceneNode::Ptr vector
	auto wreckfieldBegin2 = std::remove_if(mChildren.begin(), mChildren.end(),
		std::mem_fn(&SceneNode::isMarkedForRemoval));

	mChildren.erase(wreckfieldBegin2, mChildren.end());
	
	std::for_each(mChildren.begin(), mChildren.end(),
		std::mem_fn(&SceneNode::removeWrecks));

}

SceneNode* SceneNode::getRoot(){
	SceneNode* node = this;
	while (node->mParent != nullptr)
		node = node->mParent;

	return node;
}

int SceneNode::nbChildren() const{
	return mChildren.size();
}

std::vector<SceneNode*> SceneNode::getNodes(){
	return mNodes;
}

void SceneNode::addNode(SceneNode* node){
	getRoot()->mNodes.push_back(node);
}

SceneNode* SceneNode::getNode(int i){
	return mNodes[i];
}

void SceneNode::addNodes(){
	addNode(this);

	FOREACH(Ptr& node, mChildren)
		node->addNodes();
}

void SceneNode::clearNodes(){
	getRoot()->mNodes.clear();
}
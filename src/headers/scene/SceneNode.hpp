#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include <set>

#include "../util/Category.hpp"

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {

public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;
	
public:
	explicit SceneNode(Category::Type category = Category::None);
	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	void update(sf::Time dt, CommandQueue& commands);

	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;

	virtual unsigned int getCategory() const;
	virtual bool isDestroyed() const;
	virtual bool isMarkedForRemoval() const;

	void onCommand(const Command& command, sf::Time dt);

	virtual sf::FloatRect getBoundingRect() const;
	void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void removeWrecks();

	SceneNode* getRoot();
	int nbChildren() const;
	void addNodes();
	std::vector<SceneNode*> getNodes();
	void addNode(SceneNode* node);
	void clearNodes();

	SceneNode* getNode(int i);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateChildren(sf::Time dt, CommandQueue& commands);

protected:
	std::vector<Ptr> mChildren;
	std::vector<SceneNode*> mNodes;

	SceneNode* mParent;
	Category::Type mDefaultCategory;
};

float distance(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs);

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

#endif
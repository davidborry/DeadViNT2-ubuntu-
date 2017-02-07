#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Entity.hpp"
#include "../scene/SpriteNode.hpp"
#include "../util/PathFindingGrid.hpp"
#include "../../headers/util/SteeringBehaviours.hpp"

class Human;


class Zombie : public Entity{
public:
	Zombie(const TextureHolder& textures);

	void setTarget(Human* human);
	Human* getTarget();

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void seekPosition(sf::Vector2f location, sf::Time dt);
	void seekTarget(sf::Time dt);
	void followPath(sf::Time dt);

	bool closeToTarget();
	void setPath(Path path);
	Path getPath() const;

	void updateTargetRadar();

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateSteering();


private:
	sf::Sprite mSprite;
	Human* mTarget;
	Path mPath;
	int mCurrentPathNode;

	sf::Vector2f mSteering;

};



#endif
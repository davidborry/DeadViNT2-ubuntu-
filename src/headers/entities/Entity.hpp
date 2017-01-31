#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "../scene/SceneNode.hpp"
#include "../util/ResourceHolder.hpp"
#include "../util/ResourceIdentifier.hpp"

class Entity : public SceneNode{
public:
	explicit Entity(int hitPoints);

	void setVelocity(sf::Vector2f vector);
	void setVelocity(float vx, float vy);
	
	sf::Vector2f getVelocity() const;
	void accelerate(sf::Vector2f velocity);
	void adjustPositionObstacle(SceneNode* obstacle);

	void repair(int points);
	void damage(int points);
	virtual void destroy();
	virtual void remove();

	int getHitPoints() const;
	virtual bool isDestroyed() const;

	sf::Vector2f getDXY() const;


protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

protected:
	sf::Vector2f mVelocity;
	int mHitPoints;
};

#endif
#include "../../headers/entities/Entity.hpp"
#include "../../headers/util/Utility.hpp"

Entity::Entity(int hitPoints) : 
mHitPoints(hitPoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity){
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy){
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const{
	return mVelocity;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands){
		move(mVelocity*dt.asSeconds());
}

void Entity::accelerate(sf::Vector2f velocity){
	mVelocity += velocity;
}

void Entity::damage(int points){
	mHitPoints -= points;
	if(isDestroyed())
		destroy();
}

void Entity::repair(int points){
	mHitPoints += points;
}

void Entity::destroy(){
//	printf("Dead\n");
	mHitPoints = 0;
}

int Entity::getHitPoints() const{
	return mHitPoints;
}

bool Entity::isDestroyed() const{
	return mHitPoints <= 0;
}

void Entity::remove(){
	destroy();
}

sf::Vector2f Entity::getDXY() const{
	float angle = toRadian(getRotation());

	return sf::Vector2f(std::sin(angle), -std::cos(angle));
}

void Entity::adjustPositionObstacle(SceneNode* obstacle){
	sf::Vector2f playerTL = sf::Vector2f(getBoundingRect().left, getBoundingRect().top);
	sf::Vector2f playerDR = playerTL + sf::Vector2f(getBoundingRect().width, getBoundingRect().height);

	sf::Vector2f obstacleTL = sf::Vector2f(obstacle->getBoundingRect().left, obstacle->getBoundingRect().top);
	sf::Vector2f obstacleDR = obstacleTL + sf::Vector2f(obstacle->getBoundingRect().width, obstacle->getBoundingRect().height);

	//printf("%f,%f\n", playerTL.y, obstacleDL.y);
	float playerLeft = playerTL.x;
	float playerRight = playerDR.x;
	float playerTop = playerTL.y;
	float playerBottom = playerDR.y;

	float obstacleLeft = obstacleTL.x;
	float obstacleRight = obstacleDR.x;
	float obstacleTop = obstacleTL.y;
	float obstacleBottom = obstacleDR.y;

	sf::FloatRect inter = unionRect(getBoundingRect(), obstacle->getBoundingRect());
	//printf("%f,%f\n", inter.width, inter.height);

	if (inter.width > inter.height){
		if (playerTop < obstacleBottom && playerTop > obstacleTop)
			setPosition(getPosition().x, obstacleBottom + getBoundingRect().height / 2.f);

		else if (playerBottom > obstacleTop && playerBottom < obstacleBottom)
			setPosition(getPosition().x, obstacleTop - getBoundingRect().height / 2.f);
	}

	else if (inter.width < inter.height){
		if (playerRight > obstacleLeft && playerRight < obstacleRight)
			setPosition(obstacleLeft - getBoundingRect().width / 2.f, getPosition().y);

		else if (playerLeft < obstacleRight && playerLeft > obstacleLeft)
			setPosition(obstacleRight + getBoundingRect().width / 2.f, getPosition().y);
	}

}


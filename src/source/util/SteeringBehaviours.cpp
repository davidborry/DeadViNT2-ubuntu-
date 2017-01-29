#include "../../headers/util/SteeringBehaviours.hpp"
#include "../../headers/entities/Entity.hpp"
#include "../../headers/util/Utility.hpp"

sf::Vector2f seek(Entity& seeker, sf::Vector2f target, sf::Time dt, float speed){
	float approachRate = 200.f;
		//printf("%f,%f\n", seeker.getVelocity().x, seeker.getVelocity().y);
	sf::Vector2f targetDirection = unitVector(target - seeker.getWorldPosition());
	sf::Vector2f desiredVelocity = unitVector(targetDirection*approachRate*dt.asSeconds()+seeker.getVelocity())*speed;

	float angle = toDegree(std::atan2(desiredVelocity.y, desiredVelocity.x));
	seeker.setRotation(angle + 90.f);

	return desiredVelocity;
}

float distance2(sf::Vector2f p1, sf::Vector2f p2){
	return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
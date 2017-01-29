#include "../../headers/entities/Projectile.hpp"
#include "../../headers/scene/EmitterNode.hpp"

namespace{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Projectile::Type type, const TextureHolder& textures, float speed, float range, int damage):
Entity(1),
mType(type),
mSprite(textures.get(Resources::Textures::Projectiles)),
mSpeed(speed),
mRange(range),
mDamage(damage),
mOriginUndefined(true)

{
	
	centerOrigin(mSprite);
	
	if (isGuided()){
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));

		std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
		propellant->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(propellant));
	}

}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands){
	Entity::updateCurrent(dt, commands);

	if (mOriginUndefined){
		mOrigin = getWorldPosition();
		mOriginUndefined = false;
	}


	if (isGuided()){
		const float approachRate = 200.f;
		sf::Vector2f newVelocity = unitVector(approachRate*dt.asSeconds()*mTargetDirection+getVelocity());

		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);
		
		setRotation(toDegree(angle) + 90);
		setVelocity(newVelocity);

	}

	else{
		sf::Vector2f distance = getWorldPosition() - mOrigin;
		float rawDist = std::sqrt(distance.x*distance.x + distance.y*distance.y);
		
		if (rawDist > mRange)
			destroy();
		
	}
	
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

unsigned int Projectile::getCategory() const {
	if (mType == Projectile::Type::EnemyBullet)
		return Category::EnemyProjectile;

	else
		return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const {
	return mSpeed;
}

bool Projectile::isGuided() const{
	return mType == Missile;
}

void Projectile::guideTowards(sf::Vector2f position){
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

int Projectile::getDamage() const{
	return mDamage;
}
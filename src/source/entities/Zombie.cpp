#include "../../headers/entities/Zombie.hpp"
#include "../../headers/entities/Human.hpp"
#include "../../headers/util/Utility.hpp"

Zombie::Zombie(const TextureHolder& textures) :
Entity(20),
mSprite(textures.get(Resources::Textures::Zombie)),
mTarget(nullptr),
mCurrentPathNode(1)
{
	centerOrigin(mSprite);
}

void Zombie::updateCurrent(sf::Time dt, CommandQueue& commands){


	
	if (closeToTarget())
		seekTarget(dt);
	else
		followPath(dt);
	//printf("%f, %f\n", angle, getRotation());

	if(getWorldPosition().y > mTarget->getWorldPosition().y)
		mTarget->enemyClose(HUD::S);


//	setRotation(angle);

Entity::updateCurrent(dt,commands);
}


void Zombie::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

sf::FloatRect Zombie::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

unsigned int Zombie::getCategory() const{
	return Category::Zombie;
}

void Zombie::setTarget(Human* human){
	mTarget = human;
}

Human* Zombie::getTarget(){
	return mTarget;
}

void Zombie::seekTarget(sf::Time dt){
	
	seekPosition(mTarget->getWorldPosition(), dt);
}

void Zombie::seekPosition(sf::Vector2f position,sf::Time dt){
	mVelocity = seek(*this, position,dt, 50);
}

bool Zombie::closeToTarget() {

	int x = getWorldPosition().x / 100;
	int y = getWorldPosition().y / 100;

	int targetX = mTarget->getWorldPosition().x / 100;
	int targetY = mTarget->getWorldPosition().y / 100;

	//printf("%i,%i - %i,%i\n", x, y, targetX, targetY);
	if (x == targetX && y == targetY){
		return true;
	}

	

	return false;
}

void Zombie::setPath(Path path){
	mPath = path;
	mCurrentPathNode = 1;
}

Path Zombie::getPath() const{
	return mPath;
}

void Zombie::followPath(sf::Time dt){
	//printf("%i / %i\n", mCurrentPathNode, mPath.size());

	if (mPath.size() > 1){
	//	printf("%i\n", mPath.size());
		sf::Vector2f target = mPath[mCurrentPathNode];
		if (distance2(getWorldPosition(), target) <=10){
			if (mCurrentPathNode == mPath.size() - 1)
				return;

			mCurrentPathNode++;

			if (mCurrentPathNode >= mPath.size())
				mCurrentPathNode = mPath.size() - 1;
		}

		seekPosition(mPath[mCurrentPathNode],dt);
	}

	
	
}

void Zombie::updateSteering(){
	mVelocity = unitVector(mSteering)* 50.f;
}
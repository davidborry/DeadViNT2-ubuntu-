#include <iostream>
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

	updateTargetRadar();

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

void Zombie::updateTargetRadar() {

    sf::Vector2f p1 = mTarget->getWorldPosition() - getWorldPosition();
    sf::Vector2f p2 = sf::Vector2f(0,mTarget->getWorldPosition().y);


   float dot = p1.x*p2.x + p1.y*p2.y ;     // dot product
    float det = p1.x*p2.y - p1.y*p2.x ;     // determinant
   int angle2 = toDegree(atan2(det, dot)) + 22; // atan2(y, x) or atan2(sin, cos)

    if(angle2 < 0)
        angle2+=360;

    int f = angle2/ 45;
   // cout << f <<endl;

   if(f==0)
       mTarget->enemyClose(HUD::N,true);
       else if(f==1)
        mTarget->enemyClose(HUD::NW,true);
       else if(f==2)
       mTarget->enemyClose(HUD::W,true);
    else if (f==3)
       mTarget->enemyClose(HUD::SW,true);
    else if(f==4)
       mTarget->enemyClose(HUD::S,true);
    else if(f==5)
       mTarget->enemyClose(HUD::SE,true);
    else if(f==6)
       mTarget->enemyClose(HUD::E,true);
    else
       mTarget->enemyClose(HUD::NE,true);


   // if(getWorldPosition().y > mTarget->getWorldPosition().y)
     //   mTarget->enemyClose(HUD::S);

}

void Zombie::adjustToZombie(Zombie *z) {
	cout << "ADJUSTING ZOMBIE..." << endl;
	sf::Vector2f playerTL = sf::Vector2f(getBoundingRect().left, getBoundingRect().top);
	sf::Vector2f playerDR = playerTL + sf::Vector2f(getBoundingRect().width, getBoundingRect().height);

	sf::Vector2f obstacleTL = sf::Vector2f(z->getBoundingRect().left, z->getBoundingRect().top);
	sf::Vector2f obstacleDR = obstacleTL + sf::Vector2f(z->getBoundingRect().width, z->getBoundingRect().height);

	//printf("%f,%f\n", playerTL.y, obstacleDL.y);
	float playerLeft = playerTL.x;
	float playerRight = playerDR.x;
	float playerTop = playerTL.y;
	float playerBottom = playerDR.y;

	float obstacleLeft = obstacleTL.x;
	float obstacleRight = obstacleDR.x;
	float obstacleTop = obstacleTL.y;
	float obstacleBottom = obstacleDR.y;

	sf::FloatRect inter = unionRect(getBoundingRect(), z->getBoundingRect());
	//printf("%f,%f\n", inter.width, inter.height);

	if (inter.width > inter.height){
		if (playerTop <= obstacleBottom && playerTop > obstacleTop)
			setPosition(getPosition().x, obstacleBottom + getBoundingRect().height / 2.f);

		else if (playerBottom > obstacleTop && playerBottom < obstacleBottom)
			setPosition(getPosition().x, obstacleTop - getBoundingRect().height / 2.f);
	}

	else if (inter.width <= inter.height){
		if (playerRight > obstacleLeft && playerRight < obstacleRight)
			setPosition(obstacleLeft - getBoundingRect().width / 2.f, getPosition().y);

		else if (playerLeft <= obstacleRight && playerLeft > obstacleLeft)
			setPosition(obstacleRight + getBoundingRect().width / 2.f, getPosition().y);
	}

	//else
		//setPosition(getPosition()+sf::Vector2f(1.f,1.f));

}

void Zombie::updateSteering(){
	mVelocity = unitVector(mSteering)* 50.f;
}
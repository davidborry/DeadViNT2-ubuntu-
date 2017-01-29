#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "../util/DataTable.hpp"
#include "../util/Utility.hpp"

class Projectile : public Entity{

public:
	enum Type{
		EnemyBullet,
		AlliedBullet,
		Missile,
		TypeCount,
	};

public:
	Projectile(Type type, const TextureHolder& textures, float speed = 100.f, float range = 200.f, int damage = 25);

	void guideTowards(sf::Vector2f position);
	bool isGuided() const;

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	float getMaxSpeed() const;
	int getDamage() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
	sf::Vector2f mTargetDirection;

	float mRange;
	float mDamage;
	int mSpeed;

	sf::Vector2f mOrigin;
	bool mOriginUndefined;
};

#endif
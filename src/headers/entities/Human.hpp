#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "Entity.hpp"
#include "weapons/Gun.hpp"
#include "../game/HUD.hpp"

class Human : public Entity{
public:
	Human(const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void fire();

	bool closeEnemy(int side) const;
	void enemyClose(int side, bool close = true);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
	Gun* mGun;

	std::array<bool,8> mCloseEnemies;
};

#endif
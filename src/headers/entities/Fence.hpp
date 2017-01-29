#ifndef FENCE_HPP
#define FENCE_HPP

#include "Entity.hpp"

class Fence : public Entity{
public:
	Fence(const sf::Texture& texture);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
};

#endif
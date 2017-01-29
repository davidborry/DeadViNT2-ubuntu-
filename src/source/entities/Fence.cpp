#include "../../headers/entities/Fence.hpp"

Fence::Fence(const sf::Texture& texture) :
Entity(500),
mSprite(texture, sf::IntRect(0,0,100,100))
{
}

void Fence::updateCurrent(sf::Time dt, CommandQueue& commands){

}

void Fence::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

unsigned int Fence::getCategory() const {
	return Category::Fence;
}

sf::FloatRect Fence::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}
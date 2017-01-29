#include "../../headers/scene/SpriteNode.hpp"

SpriteNode::SpriteNode(const sf::Texture& texture, bool solid)
	: mSprite(texture),
	mSolid(solid)
{
}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	: mSprite(texture, textureRect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

sf::FloatRect SpriteNode::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void SpriteNode::setSolid(bool solid){
	mSolid = solid;
}

bool SpriteNode::isSolid() const{
	return mSolid;
}

unsigned int SpriteNode::getCategory() const{
	return isSolid() ? Category::Solid : Category::None;
}
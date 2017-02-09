#include "../../headers/scene/SpriteNode.hpp"
#include "../../headers/util/Utility.hpp"

SpriteNode::SpriteNode(const sf::Texture& texture, bool solid)
	: mSprite(texture),
	mSolid(solid)
{
	centerOrigin(mSprite);

}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	: mSprite(texture, textureRect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mVisible)
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

void SpriteNode::setOpacity(const int a) {
	mSprite.setColor(sf::Color(mSprite.getColor().r,mSprite.getColor().g,mSprite.getColor().b, std::min(a,255)));
}

int SpriteNode::getOpacity() const {
	return mSprite.getColor().a;
}
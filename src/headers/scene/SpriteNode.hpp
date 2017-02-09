#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"


class SpriteNode : public SceneNode{
public:
	explicit SpriteNode(const sf::Texture& texture, bool solid = false);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

	bool isSolid() const;
	void setSolid(bool solid);

	virtual sf::FloatRect getBoundingRect() const;
	virtual unsigned int getCategory() const;

	void setOpacity(const int a);
	int getOpacity() const;


protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::Sprite mSprite;
	bool mSolid;
};

#endif
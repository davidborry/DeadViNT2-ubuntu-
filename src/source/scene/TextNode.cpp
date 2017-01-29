#include "../../headers/scene/TextNode.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	mText.setFont(fonts.get(Resources::Fonts::Main));
	mText.setCharacterSize(20);
	setString(text);
}

void TextNode::setString(const std::string& text){
	mText.setString(text);
	centerOrigin(mText);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mText, states);
}
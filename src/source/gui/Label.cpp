#include "../../headers/gui/Label.hpp"

namespace GUI{
	Label::Label(const std::string& string, const FontHolder& fonts):
		mText(string,fonts.get(Resources::Fonts::Main),16)
	{
	}

	bool Label::isSelectable() const{
		return false;
	}

	void Label::setText(std::string text){
		mText.setString(text);
	}

	void Label::handleEvent(const sf::Event& event)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		target.draw(mText, states);
	}
}
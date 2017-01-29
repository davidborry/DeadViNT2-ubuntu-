#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "../util/ResourceHolder.hpp"
#include "../util/ResourceIdentifier.hpp"
#include <string>


namespace GUI{
	class Label : public Component{

	public:
		typedef std::shared_ptr<Label> Ptr;

	public:
		Label(const std::string& text, const FontHolder& fonts);

		virtual bool isSelectable() const;
		void setText(std::string text);

		virtual void handleEvent(const sf::Event& event);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text mText;
	};
}

#endif
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"

namespace GUI{
	
	class Container : public Component{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container(sf::Window* window);

		void pack(Component::Ptr component);

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& event);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool hasSelection() const;
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();

		void selectOnClick();

	private:
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;

		sf::Window* mWindow;
	};
}

#endif
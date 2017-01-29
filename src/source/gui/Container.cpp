#include "../../headers/gui/Container.hpp"
#include "../../headers/util/foreach.hpp"
#include "../../headers/gui/Button.hpp"
namespace GUI{
	Container::Container(sf::Window* window) :
		mChildren(),
		mWindow(window),
		mSelectedChild(-1)
	{
	}

	void Container::pack(Component::Ptr component){
		mChildren.push_back(component);

		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1);
	}

	bool Container::isSelectable() const{
		return false;
	}

	void Container::handleEvent(const sf::Event& event){
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
			mChildren[mSelectedChild]->handleEvent(event);

		else if (event.type == sf::Event::MouseMoved)
			selectOnClick();
			

		else if (event.type == sf::Event::KeyPressed){
			if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z)
				selectPrevious();

			else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				selectNext();

			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
				if (hasSelection())
					mChildren[mSelectedChild]->activate();
		}

		else if (event.type == sf::Event::MouseButtonPressed)
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		
	}

	void Container::select(std::size_t index){
		if (mChildren[index]->isSelectable()){
			if (hasSelection())
				mChildren[mSelectedChild]->deselect();

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void Container::selectPrevious(){
		if (!hasSelection())
			return;

		//Search previous component that is selectable
		int prev = mSelectedChild;
		do
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		//Select that component;
		select(prev);
	}

	void Container::selectNext(){
		if (!hasSelection())
			return;

		//Search next component that is selectable
		int next = mSelectedChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		//Select that component
		select(next);
	}

	void Container::selectOnClick(){
		sf::Vector2i coordinates = sf::Mouse::getPosition(*mWindow);

			for (std::size_t child = 0; child < mChildren.size(); child++){
				auto button = dynamic_cast<GUI::Button*>(mChildren[child].get());

				if ( button != NULL){
					sf::Vector2f position = mChildren[child]->getPosition();
					sf::Vector2u size = button->getTextureSize();
					
					//printf("%i,%i\n", size.x, size.y);

					if (coordinates.x >= position.x && coordinates.x <= position.x + size.x
						&& coordinates.y >= position.y && coordinates.y <= position.y + size.y){
						//printf("SDFSF");
						select(child);
					}
				}

			}
	}

	bool Container::hasSelection() const{
		return mSelectedChild >= 0;
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();

		FOREACH(const Component::Ptr& child, mChildren)
			target.draw(*child, states);
	}
}
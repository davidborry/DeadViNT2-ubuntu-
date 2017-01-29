#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "../gui/Button.hpp"
#include "../gui/Container.hpp"

class MenuState : public State{
public:
	MenuState(StateStack& stack, Context context, int param = 0);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};

#endif
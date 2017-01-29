#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"

class PauseState : public State{
public:
	PauseState(StateStack& stack, Context context, int param = 0);
	~PauseState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mPauseText;
	sf::Text mInstructionText;
};

#endif
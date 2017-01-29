#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"

class GameOverState : public State{

public:
	GameOverState(StateStack& stack, Context context, int params = 0);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Time mElapsedTime;
	sf::Text mGameOverText;
};

#endif
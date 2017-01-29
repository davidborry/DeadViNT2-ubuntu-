#ifndef LOADINGSTATE_HPP
#define LOADINGSTATE_HPP

#include "State.hpp"
#include "../util/ParallelTask.hpp"

class LoadingState : public State{
public:
	LoadingState(StateStack& stack, Context context, int param = 0);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	void setCompletion(float percent);

private:
	sf::Text mLoadingText;
	sf::RectangleShape mProgressBarBackground;
	sf::RectangleShape mProgressBar;

	ParallelTask mLoadingTask;
};
#endif
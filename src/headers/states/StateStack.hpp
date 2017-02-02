#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "SFML/Graphics.hpp"
#include "State.hpp"
#include <functional>
#include <assert.h>
#include "../util/foreach.hpp"
#include <iostream>

class StateStack : public sf::NonCopyable{
public:
	enum Action{
		Push,
		Pop,
		Clear,
	};

public:
	explicit StateStack(State::Context& context);

	template<typename T> 
	void registerState(States::ID stateID, int param = 0){
		mFactories[stateID] = [this,param]() {
			return State::Ptr(new T(*this, mContext,param));
		};
	}

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(States::ID stateId);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::Ptr createState(States::ID stateId);
	void applyPendingChanges();

private:
	struct PendingChange{
		explicit PendingChange(Action action, States::ID stateID = States::None);

		Action action;
		States::ID stateID;
	};

private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	State::Context& mContext;
	std::map < States::ID, std::function<State::Ptr()>> mFactories;
};

#endif
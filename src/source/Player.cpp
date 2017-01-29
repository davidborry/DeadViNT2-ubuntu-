#include "../headers/Player.hpp"
#include "../headers/scene/SceneNode.hpp"
#include "../headers/entities/Human.hpp"
#include <iostream>

Player::Player():
gameOver(false)
{
	mKeyBinding[sf::Keyboard::Q] = Action::TurnLeft;
	mKeyBinding[sf::Keyboard::D] = Action::TurnRight;
	mKeyBinding[sf::Keyboard::Z] = Action::Forward;
	mKeyBinding[sf::Keyboard::S] = Action::Backwards;
	mKeyBinding[sf::Keyboard::Space] = Action::Fire;

	initializeActions();

	FOREACH(auto& pair, mActionBinding){
		pair.second.category = Category::PlayerHuman;
	}

}


void Player::handleEvents(const sf::Event& event, CommandQueue& commands){
	if (event.type == sf::Event::KeyReleased){
		if (event.key.code == sf::Keyboard::P){
			Command output;
			output.category = Category::PlayerHuman;
			output.action = [](SceneNode& s, sf::Time){
				std::cout << s.getPosition().x << ',' << s.getPosition().y << std::endl;
			};

			commands.push(output);
		}

		
	}
}


void Player::handleRealTimeInputs(CommandQueue& commands){
	//printf("%i\n", gameOver);
	if (!gameOver)
	FOREACH(auto& pair, mKeyBinding)
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);

}

void Player::initializeActions(){
	const float playerSpeed = 200.f;
	mActionBinding[Forward].action = derivedAction<Human>([](Human& h, sf::Time){
		
		h.setVelocity(100.f*h.getDXY());
	});

	mActionBinding[Backwards].action = derivedAction<Human>([](Human& h, sf::Time){
		h.setVelocity(-100.f*h.getDXY());
	});

	mActionBinding[TurnLeft].action = derivedAction<Human>([](Human& h, sf::Time){
		h.rotate(-5.f);
	});

	mActionBinding[TurnRight].action = derivedAction<Human>([](Human& h, sf::Time){
		h.rotate(5.f);
	});

	mActionBinding[Fire].action = derivedAction<Human>([](Human& h, sf::Time){
		h.fire();
	});

}

bool Player::isRealTimeAction(Action action){
	switch (action){
	case Forward:
	case Backwards:
	case TurnLeft:
	case TurnRight:
	case Fire:
		return true;

	default:
		return false;
	}
}

void Player::assignKey(sf::Keyboard::Key key, Action action){
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();){
		if (itr->second == action)
			mKeyBinding.erase(itr++);

		else
			itr++;
	}

	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const{
	FOREACH(auto pair, mKeyBinding)
		if (pair.second == action)
			return pair.first;

	return sf::Keyboard::Unknown;
	
}

void Player::endGame(){
	gameOver = true;
}

void Player::startGame(){
	gameOver = false;
}


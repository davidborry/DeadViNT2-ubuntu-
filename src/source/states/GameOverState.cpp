#include "../../headers/states/GameOverState.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/Player.hpp"
#include "../../headers/util/Utility.hpp"

GameOverState::GameOverState(StateStack& states, Context context, int param) :
State(states, context),
mGameOverText(),
mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Resources::Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);

	mGameOverText.setString("Game Over !");

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f*windowSize.x, 0.4f*windowSize.y);
}

void GameOverState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt){
	mElapsedTime += dt;

	if (mElapsedTime > sf::seconds(3)){
		requestStackClear();
		requestStackPush(States::Menu);
	}

	return false;
}

bool GameOverState::handleEvent(const sf::Event& event){
	return false;
}
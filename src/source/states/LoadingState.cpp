#include "../../headers/states/LoadingState.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"

LoadingState::LoadingState(StateStack& stack, Context context, int param) : State(stack, context, param){
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Resources::Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setString("Loading resources");
	centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 10.f, 10.f));
	mProgressBarBackground.setPosition(10, mLoadingText.getPosition().y + 40.f);

	mProgressBar.setFillColor(sf::Color(100, 100, 100));
	mProgressBar.setSize(sf::Vector2f(200, 10));
	mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);

	setCompletion(0.f);
	mLoadingTask.execute();
}

void LoadingState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	//printf("d");
	window.draw(mLoadingText);
	window.draw(mProgressBarBackground);
	window.draw(mProgressBar);
}

bool LoadingState::update(sf::Time dt){
	if (mLoadingTask.isFinished()){
		requestStackPop();
		requestStackPush(States::Game);
	}

	else
		setCompletion(mLoadingTask.getCompletion());
	
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event){
	return true;
}

void LoadingState::setCompletion(float percent){

	//printf("%f\n", percent);
	if (percent > 1.f)
		percent = 1.f;

	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x * percent, mProgressBar.getSize().y));
	//mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);

}
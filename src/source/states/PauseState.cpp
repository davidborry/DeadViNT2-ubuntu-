#include "../../headers/states/PauseState.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"

PauseState::PauseState(StateStack& stack, Context context, int param) :
State(stack, context, param),
mBackgroundSprite(),
mPauseText(),
mInstructionText()

{

	getContext().music->setPaused(true);

	sf::Font& font = context.fonts->get(Resources::Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();

	mPauseText.setFont(font);
	mPauseText.setString("Game Paused");
	mPauseText.setCharacterSize(70);
	centerOrigin(mPauseText);
	mPauseText.setPosition(0.5f*viewSize.x, 0.4f*viewSize.y);

	mInstructionText.setFont(font);
	mInstructionText.setString("Press backspace to return to the main menu");
	centerOrigin(mInstructionText);
	mInstructionText.setPosition(0.5f*viewSize.x, 0.6f*viewSize.y);

}

PauseState::~PauseState(){
	//getContext().music->setPaused(false);
}

void PauseState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPauseText);
	window.draw(mInstructionText);

}

bool PauseState::update(sf::Time dt){
	//printf("%i\n", mParam);

	return false;
}

bool PauseState::handleEvent(const sf::Event& event){
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape)
		requestStackPop();

	else if (event.key.code == sf::Keyboard::Space){
		requestStackClear();
		requestStackPush(States::Menu);
	}

	return false;

}
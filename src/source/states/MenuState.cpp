#include "../../headers/states/MenuState.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"
#include "../../headers/util/foreach.hpp"

MenuState::MenuState(StateStack& stack, Context context, int param) : 
State(stack, context, param),
mGUIContainer(context.window)
{
	//sf::Texture& texture = context.textures->get(Resources::Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Resources::Fonts::Main);

	//mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallBack([this]() {
		requestStackPop();
		requestStackPush(States::Loading);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(100, 300);
	settingsButton->setText("Settings");
	settingsButton->setCallBack([this]() {
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(100, 350);
	exitButton->setText("Exit");
	exitButton->setCallBack([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
}

void MenuState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	window.draw(mGUIContainer);

}

bool MenuState::update(sf::Time dt){
	//printf("%i\n", mParam);

	return true;
}

bool MenuState::handleEvent(const sf::Event& event){
	mGUIContainer.handleEvent(event);
	return true;
}
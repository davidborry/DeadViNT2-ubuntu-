#include "../../headers/states/TitleState.hpp"
#include "../../headers/util/Utility.hpp"
#include "../../headers/util/ResourceHolder.hpp"

TitleState::TitleState(StateStack& stack, Context context, int param): 
State(stack,context, param),
mText(),
mShowText(true),
mTextEffectTime(sf::Time::Zero)
{
	
	//mBackgroundSprite.setTexture(context.textures->get(Resources::Textures::TitleScreen));

	mText.setFont(context.fonts->get(Resources::Fonts::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt){
	mTextEffectTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f)){
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event){
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed){
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}
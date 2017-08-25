#include "../../headers/states/SettingsState.hpp"
#include "../../headers/util/Utility.hpp"

SettingsState::SettingsState(StateStack& stack, Context& context, int param) :
State(stack, context),
mGUIContainer(context.window)
{
	addButtonLabel(Player::TurnLeft, 150.f, "Turn Left", context);
	addButtonLabel(Player::TurnRight, 200.f, "Turn Right", context);
	addButtonLabel(Player::Backwards, 250.f, "Backwards", context);
	addButtonLabel(Player::Forward, 300.f, "Forward", context);

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(80.f, 375.f);
	backButton->setText("Back");
	backButton->setCallBack([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(backButton);
	//mBackgroundSprite.setTexture(context.textures->get(Resources::Textures::TitleScreen));

}

void SettingsState::updateLabels(){
	Player& player = *getContext().player;
	for (std::size_t i = 0; i < Player::ActionCount - 3; i++){
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->setText(std::to_string(key));
	}
}


void SettingsState::draw(){
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt){
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event){
	bool isKeyBinding = false;
	for (std::size_t action = 0; action < Player::ActionCount - 3; action++){
		if (mBindingButtons[action]->isActive()){
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased){
				getContext().player->assignKey(event.key.code, static_cast<Player::Action>(action));
				mBindingButtons[action]->deactivate();
			}

			break;
		}
	}

	if (isKeyBinding)
		updateLabels();

	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context){
	mBindingButtons[action] = std::make_shared<GUI::Button>(context);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("",*context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);

}


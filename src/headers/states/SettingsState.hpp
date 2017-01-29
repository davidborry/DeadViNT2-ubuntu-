#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "State.hpp"
#include "../Player.hpp"
#include "../gui/Container.hpp"
#include "../gui/Button.hpp"
#include "../gui/Label.hpp"
#include <array>

class SettingsState : public State{
public:
	SettingsState(StateStack& stack, Context context, int param = 0);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, Player::ActionCount - 2> mBindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount - 2> mBindingLabels;
};

#endif
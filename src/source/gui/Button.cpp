#include "../../headers/gui/Button.hpp"
#include "../../headers/util/Utility.hpp"

namespace GUI{
	Button::Button(State::Context context):
		mContext(context),
		mCallback(),
		mSprite(context.textures->get(Resources::Textures::Buttons)),
		mText("", context.fonts->get(Resources::Fonts::Main)),
		mIsToggle(false),
		mSound(*context.sounds)
	{
		changeTexture(Normal);
		sf::FloatRect bounds = mSprite.getLocalBounds();
		mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}

	void Button::setCallBack(Button::Callback callback){
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string& text){
		mText.setString(text);
		centerOrigin(mText);
	}

	bool Button::isSelectable() const{
		return true;
	}

	void Button::select(){
		Component::select();
		changeTexture(Selected);
	}

	void Button::deselect(){
		Component::deselect();
		changeTexture(Normal);
	}

	void Button::activate(){
		Component::activate();

		// If we are toggle then we should show that the button is pressed and thus "toggled".
		if (mIsToggle)
			changeTexture(Pressed);

		if (mCallback)
			mCallback();

		// If we are not a toggle then deactivate the button since we are just momentarily activated.
		if (!mIsToggle)
			deactivate();

		mSound.play(Resources::SoundEffects::Button);

	}

	void Button::deactivate(){
		Component::deactivate();

		if (mIsToggle){
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event& event) {

	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		target.draw(mText, states);
	}

	void Button::setToggle(bool flag){
		mIsToggle = flag;
	}

	sf::Vector2u Button::getTextureSize() const{
		return sf::Vector2u(mSprite.getTextureRect().width,mSprite.getTextureRect().height);
	}

	void Button::changeTexture(Button::Type button){
		sf::IntRect textureRect(0, 50 * button, 200, 50);
		mSprite.setTextureRect(textureRect);
	}
}


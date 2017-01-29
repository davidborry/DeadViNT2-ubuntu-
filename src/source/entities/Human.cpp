#include "../../headers/entities/Human.hpp"
#include "../../headers/util/Utility.hpp"
Human::Human(const TextureHolder& textures):
Entity(100),
mGun(nullptr),
mSprite(textures.get(Resources::Textures::Human))
{
	centerOrigin(mSprite);

	std::unique_ptr<Gun> gun(new Gun(*this,textures,Gun::Pistol)) ;
	mGun = gun.get();
	mGun->setPosition(0.f, -25.f);
	attachChild(std::move(gun));
}

void Human::fire(){
	mGun->fire();
}

void Human::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

sf::FloatRect Human::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

unsigned int Human::getCategory() const{
	return Category::PlayerHuman;
}
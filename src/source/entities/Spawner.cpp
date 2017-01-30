//
// Created by david on 30/01/17.
//

#include "../../headers/entities/Spawner.hpp"
#include "../../headers/util/Utility.hpp"



Spawner::Spawner(const TextureHolder &textures) :
Entity(500),
mSprite(textures.get(Resources::Textures::Spawner), sf::IntRect(0,0,100,100)){

}

void Spawner::updateCurrent(sf::Time dt, CommandQueue &commands) {

}

void Spawner::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);

}

unsigned int Spawner::getCategory() const{
    return Category::Spawner;
}

sf::FloatRect Spawner::getBoundingRect() const {
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}
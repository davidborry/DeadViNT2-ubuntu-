#include "../../headers/scene/SoundNode.hpp"

SoundNode::SoundNode(SoundPlayer& player):
SceneNode(),
mSounds(player)
{
}

void SoundNode::playSound(Resources::SoundEffects::ID effect, sf::Vector2f position){
	mSounds.play(effect, position);
}

unsigned int SoundNode::getCategory() const{
	return Category::SoundEffect;
}
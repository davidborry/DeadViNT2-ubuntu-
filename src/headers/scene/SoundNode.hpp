#ifndef SOUNDNODE_HPP
#define SOUNDNODE_HPP

#include "SceneNode.hpp"
#include "../sound/SoundPlayer.hpp"
#include "../util/ResourceIdentifier.hpp"

class SoundNode : public SceneNode{

public:
	explicit SoundNode(SoundPlayer& player);
	void playSound(Resources::SoundEffects::ID effect, sf::Vector2f position);

	virtual unsigned int getCategory() const;

private:
	SoundPlayer& mSounds;
};

#endif
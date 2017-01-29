#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "SFML/System/NonCopyable.hpp"
#include "../util/ResourceIdentifier.hpp"
#include "../util/ResourceHolder.hpp"
#include "SFML/System/Vector2.hpp"
#include <list>
#include "SFML/Audio/Sound.hpp"

class SoundPlayer : private sf::NonCopyable{
public:
	SoundPlayer();

	void play(Resources::SoundEffects::ID effect);
	void play(Resources::SoundEffects::ID effect, sf::Vector2f position);

	void removeStoppedSounds();
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition() const;

private:
	soundBufferHolder mSoundBuffers;
	std::list<sf::Sound> mSounds;
};

#endif
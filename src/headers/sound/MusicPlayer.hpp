#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include "SFML/System/NonCopyable.hpp"
#include "../util/ResourceIdentifier.hpp"
#include "SFML/Audio.hpp"
#include <map>

class MusicPlayer : private sf::NonCopyable{
public:
	MusicPlayer();
	void play(Resources::Musics::ID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

private:
	sf::Music mMusic;
	std::map<Resources::Musics::ID, std::string> mFilenames;
	float mVolume;
};

#endif
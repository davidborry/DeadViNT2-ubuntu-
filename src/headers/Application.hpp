#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"
#include "util/ResourceHolder.hpp"
#include "util/ResourceIdentifier.hpp"
#include "Player.hpp"
#include "states/StateStack.hpp"
#include "sound/MusicPlayer.hpp"
#include "../headers/sound/SoundPlayer.hpp"

class Application{

public:
	Application();
	void run();

private:
	void processInputs();
	void update(sf::Time dt);
	void render();

	void updateStatistics(sf::Time dt);
	void registerStates();

private:
	static const sf::Time TimePerFrame;

	std::string mLevel;
	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	Player mPlayer;

	StateStack* mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;

	MusicPlayer mMusic;
	SoundPlayer mSounds;

	State::Context mContext;
	
};

#endif
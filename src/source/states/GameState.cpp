#include "../../headers/states/GameState.hpp"
#include "../../headers/states/StateStack.hpp"

GameState::GameState(StateStack& stack, Context& context, int param) :
State(stack, context,param),
mWorld(*context.window,*context.fonts,*context.sounds),
mPlayer(*context.player)
{

	cout << "LEVEL : " << *mContext->level << endl;
	//context.music->play(Resources::Musics::MissionTheme);

}

void GameState::draw(){
	mWorld.draw();
}

bool GameState::update(sf::Time dt){

	/*if (mWorld.gameStatus())
		mPlayer.endGame();*/

	mWorld.update(dt);
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealTimeInputs(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event){
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvents(event, commands);


	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){

		//mStack->registerState<PauseState>(States::Pause,8);
		requestStackPush(States::Pause);
	}

	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
		mWorld.printGrid();

	return true;
}
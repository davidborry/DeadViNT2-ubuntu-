#include "../../headers/states/GameState.hpp"
#include "../../headers/states/StateStack.hpp"

GameState::GameState(StateStack& stack, Context& context, int param) :
State(stack, context,param),
mPlayer(*context.player)
{

	cout << "LEVEL : " << *mContext->level << endl;
	//context.music->play(Resources::Musics::MissionTheme);

}

void GameState::draw(){
    getContext().world->draw();
}

bool GameState::update(sf::Time dt){

	/*if (mWorld.gameStatus())
		mPlayer.endGame();*/

	getContext().world->update(dt);
	CommandQueue& commands =  getContext().world->getCommandQueue();
	mPlayer.handleRealTimeInputs(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event){
	CommandQueue& commands = getContext().world->getCommandQueue();
	mPlayer.handleEvents(event, commands);


	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){

		//mStack->registerState<PauseState>(States::Pause,8);
		requestStackPush(States::Pause);
	}

	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
        getContext().world->printGrid();

	return true;
}
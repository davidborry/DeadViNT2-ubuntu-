#include "../../headers/states/State.hpp"
#include "../../headers/states/StateStack.hpp"

State::Context::Context(sf::RenderWindow& window,
						TextureHolder& textures,
						FontHolder& fonts,
						Player& player,
						MusicPlayer& music,
						SoundPlayer& sounds,
						std::string& level) :
window(&window),
textures(&textures),
fonts(&fonts),
player(&player),
music(&music),
sounds(&sounds),
level(&level)
{

}

State::State(StateStack& stack, Context& context, int param) : mStack(&stack), mContext(&context), mParam(param)
{

}

State::~State(){

}

void State::requestStackPush(States::ID stateID){
	mStack->pushState(stateID);
}

void State::requestStackPop(){
	mStack->popState();
}

void State::requestStackClear(){
	mStack->clearStates();
}

State::Context State::getContext() const{
	return *mContext;
}

void State::setParam(int param){
	mParam = param;
}
#include <iostream>
#include "../../headers/states/LoadingState.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"

#include "../../headers/World.h";

#include "../../headers/rapidjson/document.h"
#include "../../headers/rapidjson/writer.h"
#include "../../headers/rapidjson/stringbuffer.h"

using namespace rapidjson;

LoadingState::LoadingState(StateStack& stack, Context& context, int param) : State(stack, context, param){

	std::cout << "LEVEL : " << *(mContext->level) << std::endl;

	mContext->level = new std::string("map") ;

	context.world = new World(*getContext().window, *getContext().fonts, *getContext().sounds);

    loadLevel();

	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Resources::Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setString("Loading resources");
	centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 10.f, 10.f));
	mProgressBarBackground.setPosition(10, mLoadingText.getPosition().y + 40.f);

	mProgressBar.setFillColor(sf::Color(100, 100, 100));
	mProgressBar.setSize(sf::Vector2f(200, 10));
	mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);

	setCompletion(0.f);
	mLoadingTask.execute();
}

void LoadingState::draw(){
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	//printf("d");
	window.draw(mLoadingText);
	window.draw(mProgressBarBackground);
	window.draw(mProgressBar);
}

bool LoadingState::update(sf::Time dt){
	if (mLoadingTask.isFinished()){
		requestStackPop();
		requestStackPush(States::Game);
	}

	else
		setCompletion(mLoadingTask.getCompletion());
	
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event){
	return true;
}

void LoadingState::setCompletion(float percent){

	//printf("%f\n", percent);
	if (percent > 1.f)
		percent = 1.f;

	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x * percent, mProgressBar.getSize().y));
	//mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);
}

void LoadingState::loadLevel() {

    const char* text = "{\"test\": \"this is a test\", \"array\": [[1,2],[3,4],[5,6]]}";
    Document d;
    d.Parse(text);

    Value& value = d["test"];
    Value& array = d["array"];

    std::cout << value.GetString() << std::endl;


    for (SizeType i = 0; i < array.Size(); i++) {// rapidjson uses SizeType instead of size_t.
        auto point = array[i].GetArray();
        for (SizeType j = 0; j < point.Size(); j++){
            std::cout <<point[j].GetInt() <<" ";
        }

        std::cout << std::endl;

    }

    getContext().world->init();
}
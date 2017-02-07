//
// Created by david on 05/02/17.
//

#include <iostream>
#include "../../headers/game/HUD.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/Utility.hpp"
#include "../../headers/util/foreach.hpp"
#include "../../headers/scene/SpriteNode.hpp"

HUD::HUD(const TextureHolder& textures, Human* human) :
mSprite(textures.get(Resources::Textures::Radar)),
mPlayerHuman(nullptr)
{
    mPlayerHuman = human;
    centerOrigin(mSprite);
    mSprite.setScale(0.5,0.5);
    mSprite.rotate(90);

    for (std::size_t i = 0; i < RadarCount; ++i){

        SpriteNode::Ptr sprite(new SpriteNode(textures.get(Resources::Textures::Radar)));
        mRadar[i] = sprite.get();


        sprite->rotate(45.f*i + 90.f);
        sprite->setScale(0.5,0.5);
        sprite->hide();

        attachChild(std::move(sprite));


    }

    mRadar[W]->move(-200.f,-100.f);
    mRadar[E]->move(200.f,-100.f);
    mRadar[S]->move(0.f,100.f);
    mRadar[N]->move(0.f,-300.f);

    mRadar[NW]->move(-200.f,-300.f);
    mRadar[NE]->move(200.f,-300.f);
    mRadar[SE]->move(200.f,100.f);
    mRadar[SW]->move(-200.f,100.f);


}

void HUD::updateCurrent(sf::Time dt, CommandQueue &commands) {
    setPosition(mPlayerHuman->getWorldPosition() + sf::Vector2f(0.f,100.f));
    for(std::size_t i = 0; i < RadarCount; i++){
      //  cout << mPlayerHuman->closeEnemy(i) << endl;
        if(mPlayerHuman->closeEnemy(i)){
            mRadar[i]->show();
        }

        else
            mRadar[i]->hide();

        mPlayerHuman->enemyClose(i,false);
    }
}

void HUD::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
   // target.draw(mSprite, states);
}
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

        std::unique_ptr<SpriteNode> sprite(new SpriteNode(textures.get(Resources::Textures::Radar)));
        mRadar[i] = sprite.get();


        sprite->rotate(45.f*i + 90.f + 180.f);
        sprite->setScale(0.2,0.2);
        sprite->hide();
        sprite->setOpacity(0);

        attachChild(std::move(sprite));


    }

    mRadar[W]->move(-200.f,-100.f);
    mRadar[E]->move(200.f,-100.f);
    mRadar[S]->move(0.f,100.f);
    mRadar[N]->move(0.f,-300.f);

    mRadar[NW]->move(-140.f,-240.f);
    mRadar[NE]->move(140.f,-240.f);
    mRadar[SE]->move(140.f,40.f);
    mRadar[SW]->move(-140.f,40.f);

    mDisplay = false;


}

void HUD::display() {
  //  cout << "TEST";
    mDisplay = true;
}

void HUD::updateCurrent(sf::Time dt, CommandQueue &commands) {
    setPosition(mPlayerHuman->getWorldPosition() + sf::Vector2f(0.f,100.f));
    bool f = true;
    for(std::size_t i = 0; i < RadarCount; i++){
      //  cout << mPlayerHuman->closeEnemy(i) << endl;
        if( mPlayerHuman->closeEnemy(i)){
            mRadar[i]->show();
        }

        else
            mRadar[i]->hide();

        manageOpacity(i,f,dt);
        //cout << mDisplayTime.asMilliseconds()/10000.f << endl;

        mPlayerHuman->enemyClose(i,false);
    }
}


void HUD::manageOpacity(const int& i, bool& f, sf::Time dt) {
    if(mDisplay) {
        mRadar[i]->setOpacity(std::min(mRadar[i]->getOpacity()+4,200));

        if (mDisplayTime > sf::seconds( 5.f)) {
            //  cout << "TIME" << endl;
            mDisplayTime = sf::Time::Zero;
            mDisplay = false;
        }

        if(f) {
            mDisplayTime += dt;
            f = false;
        }
    }

    else {
        mRadar[i]->setOpacity(std::max(mRadar[i]->getOpacity() -4,0));
    }

}


void HUD::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
   // target.draw(mSprite, states);
}

unsigned int HUD::getCategory() const {
    return Category::HUD;
}
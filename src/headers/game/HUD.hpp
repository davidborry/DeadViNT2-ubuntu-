//
// Created by david on 05/02/17.
//

#ifndef DEADVINT_2_HUD_HPP_H
#define DEADVINT_2_HUD_HPP_H

#include "../util/ResourceIdentifier.hpp"
#include "../entities/Human.hpp"
#include "../scene/SpriteNode.hpp"

class HUD : public SceneNode {
public:
    explicit HUD(const TextureHolder& textures, Human* human);
    virtual unsigned int getCategory() const;

    void display();


protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    void manageOpacity(const int& i, bool& f, sf::Time dt);

public:
    enum Radar{
        W,
        NW,
        N,
        NE,
        E,
        SE,
        S,
        SW,
        RadarCount
    };

private:
    sf::Sprite mSprite;
    Human* mPlayerHuman;

    std::array<SpriteNode*, RadarCount> mRadar;
    sf::Time mDisplayTime = sf::Time::Zero;
    bool mDisplay = false;

};

#endif //DEADVINT_2_HUD_HPP_H

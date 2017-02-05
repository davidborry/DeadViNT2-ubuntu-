//
// Created by david on 05/02/17.
//

#ifndef DEADVINT_2_HUD_HPP_H
#define DEADVINT_2_HUD_HPP_H

#include "../scene/SceneNode.hpp"
#include "../util/ResourceIdentifier.hpp"
#include "../entities/Human.hpp"

class HUD : public SceneNode {
public:
    explicit HUD(const TextureHolder& textures, Human* human);

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

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

    std::array<SceneNode*, RadarCount> mRadar;

};

#endif //DEADVINT_2_HUD_HPP_H

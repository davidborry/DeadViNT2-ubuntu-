#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "Entity.hpp"

class Spawner : public Entity{
public:
    Spawner(const TextureHolder& textures);

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;

};

#endif
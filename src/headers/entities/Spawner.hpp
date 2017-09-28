#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "Entity.hpp"
#include "Zombie.hpp"
#include "../commands/Command.hpp"

class Spawner : public Entity{
public:
    Spawner(const TextureHolder& textures, std::vector<Zombie*>& zombies, Human* human, PathFindingGrid* pathFindingGrid);

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;

    Path getPath() const;

    void updatePath();
    void setGridPosition(int x, int y);
    void spawn(SceneNode& node, const TextureHolder& textures);
    void push(int i);

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
    Human* mPlayerHuman;
    std::vector<Zombie*>& mActiveEnemies;
    Command mSpawnCommmand;

    PathFindingGrid* mPathFindingGrid;
    PathFindingGrid::Position mPlayerGridPosition;
    PathFindingGrid::Position mSpawnerGridPosition;
    Path mPath;

    sf::Time mElapsedTime;
    float mSpawnFrequency;


    int n = 0;

};

#endif
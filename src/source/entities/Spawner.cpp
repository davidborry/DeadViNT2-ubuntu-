//
// Created by david on 30/01/17.
//

#include <iostream>
#include "../../headers/entities/Spawner.hpp"
#include "../../headers/util/Utility.hpp"
#include "../../headers/commands/CommandQueue.hpp"
#include "../../headers/entities/Human.hpp"



Spawner::Spawner(const TextureHolder &textures, std::vector<Zombie*>& zombies, Human* human, PathFindingGrid* pathFindingGrid) :
Entity(500),
mActiveEnemies(zombies),
mSprite(textures.get(Resources::Textures::Spawner), sf::IntRect(0,0,100,100)),
mPlayerHuman(human),
mPathFindingGrid(pathFindingGrid)
{

    mSpawnCommmand.category = Category::SceneAirLayer;
    mSpawnCommmand.action = [this, &textures] (SceneNode& node, sf::Time) {
        spawn(node, textures);
    };

    cout << mPlayerGridPosition.x << " - " << mPlayerGridPosition.y << endl;



}

void Spawner::spawn(SceneNode& node, const TextureHolder& textures) {


    int x = mSpawnerGridPosition.x, y = mSpawnerGridPosition.y;
    std::unique_ptr<Zombie> zombie(new Zombie(textures));
    zombie->setPosition(100*x+50,100*y+50);

    zombie->setTarget(mPlayerHuman);
    zombie->setPath(mPath);

    mActiveEnemies.push_back(zombie.get());
    node.attachChild(std::move(zombie));

   // std::cout << "SPAWN : " << mActiveEnemies.size() << std::endl;

}

void Spawner::updateCurrent(sf::Time dt, CommandQueue &commands) {
    if(n > 0) {
        n--;
       // cout << " N : " << n-- << endl;
        commands.push(mSpawnCommmand);
    }
}

void Spawner::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);

}

void Spawner::updatePath() {
    mPlayerGridPosition.x = mPlayerHuman->getWorldPosition().x / 100;
    mPlayerGridPosition.y = mPlayerHuman->getWorldPosition().y / 100;

    mPath = mPathFindingGrid->findPath(mSpawnerGridPosition,mPlayerGridPosition);

    //cout << "PATH : " << mPath.size() << endl;
}

unsigned int Spawner::getCategory() const{
    return Category::Spawner;
}

sf::FloatRect Spawner::getBoundingRect() const {
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Spawner::push(int i) {
    n+=i;
}

void Spawner::setGridPosition(int x, int y) {
    setPosition(100*x,100*y);
    mSpawnerGridPosition = {x, y};

    updatePath();
}

Path Spawner::getPath() const {
    return mPath;
}
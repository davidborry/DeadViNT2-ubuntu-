//
// Created by david on 31/01/17.
//

#include <iostream>
#include "../../headers/game/ZombieManager.hpp"
#include "../../headers/entities/Human.hpp"
#include "../../headers/util/foreach.hpp"

namespace std
{
    //namespace tr1
    //{
    // Specializations for unordered containers

    template <>
    struct hash<PathFindingGrid::Position> : public unary_function<PathFindingGrid::Position, size_t>
    {
        size_t operator()(const PathFindingGrid::Position& value) const
        {
            return 0;
        }
    };

    //} // namespace tr1

    template <>
    struct equal_to<PathFindingGrid::Position> : public unary_function<PathFindingGrid::Position, bool>
    {
        bool operator()(const PathFindingGrid::Position& x, const PathFindingGrid::Position& y) const
        {
            return x.x==y.x && x.y == y.y;
        }
    };

}

ZombieManager::ZombieManager(Human* human, std::vector<Zombie*>& activeEnemies,  TextureHolder& textures, SceneNode* layer) :
mActiveEnemies(activeEnemies),
mPlayerHuman(human),
mTextures(textures),
mLayer(layer),
mPathfindingGrid(nullptr),
mRound(1),
mNewRound(false)
{

    mPlayerGridPosition.x = mPlayerHuman->getWorldPosition().x / 100;
    mPlayerGridPosition.y = mPlayerHuman->getWorldPosition().y / 100;


    printf("Initialized Zombie Manager\n");

}


void ZombieManager::update(sf::Time dt) {
    //cout << "ENEMIES : " << mActiveEnemies.size() << endl;


    if(mActiveEnemies.size() == 0 && !mNewRound){
        cout << "ENEMIES : " << mActiveEnemies.size() << endl;

        bool flag = false;
        FOREACH(auto spawner, mSpawners) {
                        if(spawner->getPath().size() > 1) {
                            if(!flag) flag = true;
                            spawner->push(mRound);

                        }
                    }

       if(flag ) {
           mRound++;
           mNewRound = true;
       }
    }

    else if(mNewRound && mActiveEnemies.size() > 0)
        mNewRound = false;



   // cout << " ROUND " << mRound << endl;
    updatePlayerGridPosition();
}

void ZombieManager::addSpawner(int x, int y) {


    std::unique_ptr<Spawner> spawner(new Spawner(mTextures, mActiveEnemies, mPlayerHuman, mPathfindingGrid));
    spawner->setGridPosition(x, y);

    mSpawners.push_back(spawner.get());
   // spawner->push(10);
    mLayer->attachChild(std::move(spawner));

}

void ZombieManager::updatePlayerGridPosition() {
    PathFindingGrid::Position a = { mPlayerHuman->getWorldPosition().x / 100, mPlayerHuman->getWorldPosition().y / 100 };
    if (a.x != mPlayerGridPosition.x || a.y != mPlayerGridPosition.y){
        //printf("%i,%i\n", a.x, a.y);
        mPlayerGridPosition = a;
        updateEnemiesPath();
        updateSpawnersPath();
    }
}

void ZombieManager::updateEnemiesPath() {
    std::unordered_map<PathFindingGrid::Position,Path> startPoints;

    FOREACH(auto zombie, mActiveEnemies){
        int x = zombie->getWorldPosition().x / 100;
        int y = zombie->getWorldPosition().y / 100;

       // printf("Zombie : %i %i \n", x, y);
        if (!startPoints.count({ x, y })){
            startPoints[{x, y}] = mPathfindingGrid->findPath({ x, y }, mPlayerGridPosition);
        }

                  //  cout << "PATH : " << startPoints[{x,y}].size() << endl;
        zombie->setPath(startPoints[{x,y}]);

    }
}

void ZombieManager::updateSpawnersPath() {
    FOREACH(auto spawner, mSpawners)
        spawner->updatePath();
}

void ZombieManager::setPathFindingGrid(PathFindingGrid& grid) {
    mPathfindingGrid = &grid;
}
#include <iostream>
#include "../headers/World.h"
#include "../headers/entities/Spawner.hpp"

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds) :
mTarget(outputTarget),
mFonts(fonts),
mSounds(sounds),
mWorldView(outputTarget.getDefaultView()),
mScrollSpeed(-50.f),
mPlayerHuman(nullptr),
gameOver(false),
mCollisionGrid(0,0),
mPathfindingGrid(0,0),
mHUD(nullptr)
{

	loadTextures();
   // cout << "WORLD" << endl;

}

World::World(const World& obj):
mTarget(obj.mTarget),
mFonts(obj.mFonts),
mSounds(obj.mSounds),
mWorldBounds(obj.mWorldBounds),
mWorldView(obj.mWorldView),
mScrollSpeed(-50.f),
mPlayerHuman(obj.mPlayerHuman),
mSpawnPosition(obj.mSpawnPosition),
gameOver(obj.gameOver),
mCollisionGrid(obj.mCollisionGrid),
mPathfindingGrid(obj.mPathfindingGrid),
mHUD(obj.mHUD),
mSceneLayers(obj.mSceneLayers)
{




    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

    loadTextures();

	for (std::size_t i = 0; i < LayerCount; ++i){

		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(*mSceneLayers[i]));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}


    std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
    mSceneGraph.attachChild(std::move(soundNode));

	mZombieManager = new ZombieManager(mPlayerHuman, mActiveEnemies, mTextures, mSceneLayers[UpperAir]);
	mZombieManager->setPathFindingGrid(mPathfindingGrid);


}

void World::init(const string& map){
    mMap = map;

    loadMap();
	buildScene();

	//testCollisions();
	loadSolids();
	testZombies();
	mWorldView.setCenter(mSpawnPosition);
}

void World::loadMap() {
    mMapParser = new MapParser(mMap);
    mMapParser->getData();

    int width = mMapParser->getWidth();
    int height = mMapParser->getHeight();

	Point spawnPoint = mMapParser->getSpawnPoint();

    mWorldBounds = sf::FloatRect(0.f, 0.f, width*CELL_WIDTH, height*CELL_WIDTH);
    mSpawnPosition = sf::Vector2f(spawnPoint.x*CELL_WIDTH, spawnPoint.y*CELL_WIDTH);

    mCollisionGrid = CollisionGrid(width*CELL_WIDTH,height*CELL_WIDTH);
    mPathfindingGrid = PathFindingGrid(width,height);

    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

}

void World::loadTextures(){

	mTextures.load(Resources::Textures::Grass, "Resources/img/Grass.jpg");
	mTextures.load(Resources::Textures::Particle, "Resources/img/Particle.png");
	mTextures.load(Resources::Textures::Explosion, "Resources/img/Explosion.png");
	mTextures.load(Resources::Textures::Human, "Resources/img/Human.png");
    mTextures.load(Resources::Textures::Weapons, "Resources/img/Weapons.png");
	mTextures.load(Resources::Textures::Projectiles, "Resources/img/Bullet.png");
	mTextures.load(Resources::Textures::Solid, "Resources/img/solid/wall.png");
	mTextures.load(Resources::Textures::Zombie, "Resources/img/Zombie.png");
	mTextures.load(Resources::Textures::Fence, "Resources/img/Fence.png");
	mTextures.load(Resources::Textures::Spawner, "Resources/img/Spawner.png");
    mTextures.load(Resources::Textures::Radar,"Resources/img/Arc.png");

}

void World::buildScene(){


	//Initialize each layer
	for (std::size_t i = 0; i < LayerCount; ++i){

		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}


	//Initialize desert sprite node
	sf::Texture& texture = mTextures.get(Resources::Textures::Grass);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Human> human(new Human(mTextures));
	human->setPosition(mSpawnPosition);
	mPlayerHuman = human.get();
	mSceneLayers[UpperAir]->attachChild(std::move(human));

	std::unique_ptr<HUD> hud(new HUD(mTextures, mPlayerHuman));
	mHUD = hud.get();
	mSceneLayers[HudLayer]->attachChild(std::move(hud));

	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

    mZombieManager = new ZombieManager(mPlayerHuman, mActiveEnemies, mTextures, mSceneLayers[UpperAir]);
	mZombieManager->setPathFindingGrid(mPathfindingGrid);


}

void World::draw(){

	if (PostEffect::isSupported()){

		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);

		//drawGrid(100,100);

	}

	else{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	}
}

void World::update(sf::Time dt){
	//printf("%i\n", mSceneGraph.getNodes().size());
	
	//forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

    updateActiveEnemies();

	mSceneGraph.removeWrecks();
	//mSceneGraph.clearNodes();

	mSceneGraph.update(dt, mCommandQueue);
    mZombieManager->update(dt);

    mPlayerHuman->setVelocity(0.f, 0.f);
	updateSounds();

	handleCollisions();

	//printf("%i\n", mActiveEnemies.size());

	mWorldView.setCenter(mPlayerHuman->getWorldPosition());

}

CommandQueue& World::getCommandQueue(){
	return mCommandQueue;
}

sf::FloatRect World::getViewBounds() const {
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

void World::handleCollisions(){
	std::set < SceneNode::Pair > collisionPairs;
	//mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	mSceneGraph.addNodes();
	std::vector<SceneNode*> nodes = mSceneGraph.getNodes();
	//printf("%i\n", nodes.size());
	mCollisionGrid.build(nodes);

	mCollisionGrid.checkCollisions(collisionPairs);
	FOREACH(SceneNode::Pair pair, collisionPairs){
		if (matchesCategories(pair, Category::Projectile, Category::Solid)){
			//printf("Solid\n");
			auto projectile = static_cast<Projectile*>(pair.first);
			projectile->destroy();
		}

        else if(matchesCategories(pair,Category::Projectile, Category::Fence)){
            auto fence = static_cast<Fence*>(pair.second);
            int x = fence->getWorldPosition().x / 100, y = fence->getWorldPosition().y / 100;
                mPathfindingGrid.setSolid(x,y,false);
                mZombieManager->updateEnemiesPath();
                mZombieManager->updateSpawnersPath();
                fence->destroy();
        }

		else if (matchesCategories(pair, Category::PlayerHuman, Category::Obstacle))
			mPlayerHuman->adjustPositionObstacle(pair.second);

		//else if (matchesCategories(pair, Category::Zombie, Category::Solid)){
        else if (matchesCategories(pair, Category::Zombie, Category::Obstacle)){
            auto zombie = static_cast<Zombie*>(pair.first);
			zombie->adjustPositionObstacle(pair.second);
		}
		

		else if (matchesCategories(pair, Category::Projectile, Category::Zombie)){
			auto zombie = static_cast<Zombie*>(pair.second);
			auto projectile = static_cast<Projectile*>(pair.first);

			zombie->damage(projectile->getDamage());
			projectile->destroy();
		}

		else if (matchesCategories(pair, Category::PlayerHuman, Category::Zombie)){
			auto zombie = static_cast<Zombie*>(pair.second);
			zombie->adjustPositionObstacle(mPlayerHuman);
		}

		else if (matchesCategories(pair, Category::Zombie, Category::Zombie)){
			auto z1 = static_cast<Zombie*>(pair.first);
			auto z2 = static_cast<Zombie*>(pair.second);

			z1->adjustPositionObstacle(z2);
		}
	}

	mSceneGraph.clearNodes();
	//printf("%i\n", collisionPairs.size());
}

bool World::gameStatus() const{
	return gameOver;
}

void World::updateSounds(){
	mSounds.setListenerPosition(mPlayerHuman->getWorldPosition());
	mSounds.removeStoppedSounds();
}


//Much faster in Release mode
void World::testCollisions(){
	sf::Texture& texture = mTextures.get(Resources::Textures::Projectiles);
	sf::IntRect textureRect(0, 0, 20, 20);
	texture.setRepeated(true);

	for (float j = 0; j < 720.f; j+=50.f)
		for (float i = 0.f; i < 1024.f; i += 100.f){
			std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
			backgroundSprite->setPosition(i, j);
			mSceneLayers[UpperAir]->attachChild(std::move(backgroundSprite));
		}
	
}

void World::drawGrid(int width, int height){
	
	for (int i = 0; i < 720; i += height){
		sf::RectangleShape line(sf::Vector2f(1024, 2));
		line.setPosition(0,i);
		mTarget.draw(line);
	}
		
	for (int i = 0; i < 1024; i += width){
		sf::RectangleShape line(sf::Vector2f(720, 2));
		line.rotate(90);
		line.setPosition(i, 0);
		mTarget.draw(line);
	}

}

void World::loadSolids(){
	//addObstacle(6, 7);

	vector<Point> obstacles = mMapParser->getObstacles();

	for(int i = 0;  i< obstacles.size(); i++)
		addObstacle(obstacles[i].x,obstacles[i].y);

  /*  for(int i = 0; i < 5; i++){
        addObstacle(i+35, 6);
        addObstacle(i+35, 8);
    }


    addObstacle(39,7);

	for (int i = 0; i < 5; i++){
			addObstacle(i+30, 5);

			if (i!=1)
			addObstacle(i+30, 15);
	}

	for (int i = 0; i <= 10; i++){
		addObstacle(30, i+5);
		if (i != 2)
			addObstacle(35, i+5);
	}

	addFence(31, 15);
	addFence(35, 7);*/

}

void World::testZombies(){


    mZombieManager->addSpawner(38,7);
	//mZombieManager->addSpawner(38,9);
    mZombieManager->addSpawner(33,9);

	mZombieManager->addSpawner(34,16);


}

void World::addObstacle(int x, int y){
	sf::Texture& texture = mTextures.get(Resources::Textures::Solid);
	sf::IntRect textureRect(0, 0, 100, 100);

		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(100*x,100*y);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));

	mPathfindingGrid.setSolid(x, y, true);

}

void World::addFence(int x, int y){
	sf::Texture& texture = mTextures.get(Resources::Textures::Fence);
	texture.setRepeated(true);

	std::unique_ptr<Fence> fence(new Fence(texture));
	fence->setPosition(100 * x, 100 * y);
	mSceneLayers[UpperAir]->attachChild(std::move(fence));

    mPathfindingGrid.setSolid(x, y, true);

}

void World::printGrid(){
	//mPathfindingGrid.print();

    PathFindingGrid::Position mPlayerGridPosition = {mPlayerHuman->getWorldPosition().x/100, mPlayerHuman->getWorldPosition().y/100};
    cout << "GRID " << mPlayerGridPosition.x << " : " << mPlayerGridPosition.y << endl;

	std::vector<sf::Vector2f> path = mPathfindingGrid.findPath({ 32,16 }, mPlayerGridPosition);

    cout << "LAST : " << path.back().x << " : " <<path.back().y << endl;
	printf("PATH : %i\n", path.size());
	printf("\n");

	sf::Texture& texture = mTextures.get(Resources::Textures::Particle);
	sf::IntRect textureRect(0, 0, 25, 25);

	for (int i = 0; i < path.size(); i++){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(path[i].x, path[i].y);
		sprite->setSolid(false);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}
}

void World::updateActiveEnemies(){
	auto wreckfieldBegin1 = std::remove_if(mActiveEnemies.begin(), mActiveEnemies.end(),
	std::mem_fn(&SceneNode::isMarkedForRemoval));

	mActiveEnemies.erase(wreckfieldBegin1, mActiveEnemies.end());

}

sf::RenderTarget& World::getTarget() {
    return mTarget;
}

FontHolder& World::getFonts() {
    return mFonts;
}

SoundPlayer& World::getSounds() {
    return mSounds;
}
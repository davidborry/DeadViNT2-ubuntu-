#include <iostream>
#include "../headers/World.h"
#include "../headers/entities/Spawner.hpp"

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

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds) :
mTarget(outputTarget),
mFonts(fonts),
mSounds(sounds),
mWorldView(outputTarget.getDefaultView()),
mWorldBounds(0.f, 0.f, 10000, 10000),
mSpawnPosition(3100.f, 700.f),
mScrollSpeed(-50.f),
mPlayerHuman(nullptr),
gameOver(false),
mCollisionCell(),
mCollisionGrid(10000,10000,1000,1000),
mPathfindingGrid(100,100)
{

    mSceneTexture.create(outputTarget.getSize().x, outputTarget.getSize().y);

	loadTextures();
	buildScene();

	mPlayerGridPosition.x = mPlayerHuman->getWorldPosition().x / 100;
	mPlayerGridPosition.y = mPlayerHuman->getWorldPosition().y / 100;

	//testCollisions();
	testSolids();
	testZombies();
	mWorldView.setCenter(mSpawnPosition);

	
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

	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

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

	mPlayerHuman->setVelocity(0.f, 0.f);
	updateSounds();

	handleCollisions();

	updatePlayerGridPosition();
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

		else if (matchesCategories(pair, Category::PlayerHuman, Category::Obstacle))
			mPlayerHuman->adjustPositionObstacle(pair.second);

		else if (matchesCategories(pair, Category::Zombie, Category::Solid)){
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

void World::testSolids(){
	//addObstacle(6, 7);

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
	addFence(35, 7);
	

	/*for (int i = 1; i < 9; i++)
		if (i != 5 && i != 4){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(200, 100 + 50*i);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}

	for (int i = 0; i < 9; i++){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(700, 100 + 50 * i);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}*/
}

void World::testZombies(){


	   spawnZombie(32, 15);
		addSpawner(32,15);

	addSpawner(38,7);
	spawnZombie(38, 7);



	
	//spawnZombie(3, 1);

}

void World::addSpawner(int x, int y) {


	std::unique_ptr<Spawner> spawner(new Spawner(mTextures));
	spawner->setPosition(100 * x, 100 * y);
	mSceneLayers[UpperAir]->attachChild(std::move(spawner));

}

void World::spawnZombie(int x, int y){
	std::unique_ptr<Zombie> zombie(new Zombie(mTextures));
	zombie->setPosition(100*x+50,100*y+50);
	

	std::vector<sf::Vector2f> path = mPathfindingGrid.findPath({ x,y }, mPlayerGridPosition);
	zombie->setTarget(mPlayerHuman);
	zombie->setPath(path);

	mActiveEnemies.push_back(zombie.get());
	mSceneLayers[UpperAir]->attachChild(std::move(zombie));

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
}

void World::printGrid(){
	//mPathfindingGrid.print();


	std::vector<sf::Vector2f> path = mPathfindingGrid.findPath({ 99,99 }, mPlayerGridPosition);
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

void World::updatePlayerGridPosition(){
	PathFindingGrid::Position a = { mPlayerHuman->getWorldPosition().x / 100, mPlayerHuman->getWorldPosition().y / 100 };
	if (a.x != mPlayerGridPosition.x || a.y != mPlayerGridPosition.y){
		printf("%i,%i\n", a.x, a.y);
		mPlayerGridPosition = a;
		updateEnemiesPath();
	}
}

void World::updateActiveEnemies(){
	auto wreckfieldBegin1 = std::remove_if(mActiveEnemies.begin(), mActiveEnemies.end(),
	std::mem_fn(&SceneNode::isMarkedForRemoval));

	mActiveEnemies.erase(wreckfieldBegin1, mActiveEnemies.end());

}

void World::updateEnemiesPath(){
	std::unordered_map<PathFindingGrid::Position,Path> startPoints;
	FOREACH(auto zombie, mActiveEnemies){
		int x = zombie->getWorldPosition().x / 100;
		int y = zombie->getWorldPosition().y / 100;

	
			//printf("CLOSE\n");
			if (!startPoints.count({ x, y })){
				startPoints[{x, y}] = mPathfindingGrid.findPath({ x, y }, mPlayerGridPosition);
			}

			zombie->setPath(startPoints[{x, y}]);

		
		
		//printf("%i\n", startPoints.size());
	//	std::vector<sf::Vector2f> path = mPathfindingGrid.findPath({ x, y }, mPlayerGridPosition);
	//	zombie->setPath(path);
	}
}
#ifndef WORLD_H
#define WORLD_H

#include "SFML/Graphics.hpp"
#include "util/ResourceHolder.hpp"
#include "util/ResourceIdentifier.hpp"
#include "scene/SceneNode.hpp"
#include "scene/SpriteNode.hpp"
#include "commands/CommandQueue.hpp"
#include "util/foreach.hpp"
#include "entities/Pickup.hpp"
#include "scene/ParticleNode.hpp"
#include "graphics/BloomEffect.hpp"
#include "sound/SoundPlayer.hpp"
#include "scene/SoundNode.hpp"
#include "entities/Human.hpp"
#include <array>
#include "util/CollisionGrid.hpp"
#include "entities/Projectile.hpp"
#include "entities/Zombie.hpp"
#include "util/PathFindingGrid.hpp"
#include "entities/Fence.hpp"
#include "game/ZombieManager.hpp"

const float CELL_WIDTH = 100.f;



class World : private sf::NonCopyable{


public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
    World(const World& obj);
	void init();
	void update(sf::Time dt);
	void draw();
	void updateSounds();

	CommandQueue& getCommandQueue();
	bool gameStatus() const;

	void addObstacle(int x, int y);
	void addFence(int x, int y);

	void printGrid();

private:
	void loadTextures();
	void buildScene();

	sf::FloatRect getViewBounds() const;

	void handleCollisions();
	void drawGrid(int width, int height);

	void testCollisions();
	void testSolids();
	void testZombies();

	void updateActiveEnemies();

    sf::RenderTarget& getTarget();
    FontHolder& getFonts();
    SoundPlayer& getSounds();

private:
	enum Layer{
		Background,
		LowerAir,
		UpperAir,
		HudLayer,
		LayerCount
	};

private:
	sf::View mWorldView;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	BloomEffect mBloomEffect;
	SoundPlayer& mSounds;
    ZombieManager* mZombieManager;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Human* mPlayerHuman;

	CommandQueue mCommandQueue;

	bool gameOver;
	
	CollisionGrid mCollisionGrid;
	PathFindingGrid mPathfindingGrid;

	std::vector<Zombie*> mActiveEnemies;

	HUD* mHUD;

};


#endif
#include <iostream>
#include "../../../headers/entities/weapons/Gun.hpp"
#include "../../../headers/util/Utility.hpp"
#include "../../../headers/util/DataTable.hpp"
#include "../../../headers/commands/CommandQueue.hpp"
#include "../../../headers/entities/Projectile.hpp"
#include "../../../headers/entities/Human.hpp"
#include "../../../headers/scene/SoundNode.hpp"
namespace{
	std::vector<GunData> Table = initializeGunData();
}

Gun::Gun(Human& human,const TextureHolder& textures, Type type) :
mParent(&human),
Entity(1),
mType(type),
mSprite(textures.get(Table[type].texture)),
mIsFiring(false),
mElapsedTime(sf::Time::Zero),
mFireCommand()
{
	cout << mType << endl;
	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneAirLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time){
		createBullets(node,textures);
	};
}


void Gun::fire(){
	mIsFiring = true;
}

void Gun::updateCurrent(sf::Time dt, CommandQueue& commands){
	
	if (mIsFiring && mElapsedTime >= sf::seconds(1.f /(Table[mType].fireRate + 1))){

		playLocalSound(commands, Resources::SoundEffects::Gunfire);
			commands.push(mFireCommand);
			mIsFiring = false;
			mElapsedTime = sf::Time::Zero;
	}

	mElapsedTime += dt;
	mIsFiring = false;
}

void Gun::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

sf::FloatRect Gun::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Gun::createBullets(SceneNode& node, const TextureHolder& textures) const{
	
	std::unique_ptr<Projectile> projectile(new Projectile(Projectile::AlliedBullet, textures,100.f,Table[mType].range,Table[mType].damage));
	projectile->setPosition(getWorldPosition());
	projectile->setVelocity(400.f*mParent->getDXY());
	node.attachChild(std::move(projectile));
}

void Gun::playLocalSound(CommandQueue& commands, Resources::SoundEffects::ID effect){
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time)
	{
		node.playSound(effect, worldPosition);
	});

	commands.push(command);
}

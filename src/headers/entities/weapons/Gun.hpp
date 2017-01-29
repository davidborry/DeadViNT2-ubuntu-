#ifndef GUN_HPP
#define GUN_HPP

#include "../Entity.hpp"
#include "../../../headers/commands/Command.hpp"
class Human;

class Gun : public Entity{

public:
	enum Type{
		Pistol,
		TypeCount
	};

public:
	Gun(Human& human,const TextureHolder& textures, Type type);

	void fire();
	virtual sf::FloatRect getBoundingRect() const;

	void createBullets(SceneNode& node, const TextureHolder& textures) const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void playLocalSound(CommandQueue& commands, Resources::SoundEffects::ID effect);

private:
	sf::Sprite mSprite;
	Type mType;

	bool mIsFiring;
	sf::Time mElapsedTime;

	Command mFireCommand;
	Human* mParent;
};

#endif
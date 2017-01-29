#include "../../headers/scene/EmitterNode.hpp"
#include "../../headers/commands/CommandQueue.hpp"

EmitterNode::EmitterNode(Particle::Type type) :
mType(type)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands){
	if (mParticleSystem)
		emittParticles(dt);

	else{
		auto finder = [this](ParticleNode& container, sf::Time){
			if (container.getParticleType() == mType)
				mParticleSystem = &container;
		};

		Command command;
		command.action = derivedAction<ParticleNode>(finder);

		command.category = Category::ParticleSystem;

		commands.push(command);
	}
}

void EmitterNode::emittParticles(sf::Time dt){
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	mAccumulatedTime += dt;

	while (mAccumulatedTime > interval){
		mAccumulatedTime -= interval;
		mParticleSystem->addParticle(getWorldPosition());
	}
}
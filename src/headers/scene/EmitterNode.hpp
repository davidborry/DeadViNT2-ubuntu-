#ifndef EMITTERNODE_HPP
#define EMITTERNODE_HPP

#include "ParticleNode.hpp"

class EmitterNode : public SceneNode{
public:
	explicit EmitterNode(Particle::Type type);

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	
	void emittParticles(sf::Time dt);

private:
	sf::Time mAccumulatedTime;
	Particle::Type mType;
	ParticleNode* mParticleSystem;
};

#endif
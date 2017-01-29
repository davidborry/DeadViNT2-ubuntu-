#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

#include "SceneNode.hpp"
#include "../graphics/Particle.hpp"
#include "../util/ResourceIdentifier.hpp"
#include <deque>

class ParticleNode : public SceneNode{
public:
	ParticleNode(Particle::Type type, const TextureHolder& textures);
	void addParticle(sf::Vector2f position);
	Particle::Type getParticleType() const;
	virtual unsigned int getCategory() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	void addVertex(float worldX, float worldY, float texCoordX, float textCoordY, sf::Color& color) const;
	void computeVertex() const;

private:
	std::deque<Particle> mParticles;
	const sf::Texture& mTexture;
	Particle::Type mType;

	mutable sf::VertexArray mVertexArray;
	mutable bool mNeedVertexUpdate;
};

#endif
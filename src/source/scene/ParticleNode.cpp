#include "../../headers/scene/ParticleNode.hpp"
#include "../../headers/util/ResourceHolder.hpp"
#include "../../headers/util/DataTable.hpp"
#include "../../headers/util/foreach.hpp"

namespace{
	std::vector<ParticleData> Table = initializeParticleData();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures):
SceneNode(),
mParticles(),
mTexture(textures.get(Resources::Textures::Particle)),
mType(type),
mVertexArray(sf::Quads),
mNeedVertexUpdate(true)
{
}

void ParticleNode::addParticle(sf::Vector2f position){
	Particle particle;
	particle.position = position;
	particle.color = Table[mType].color;
	particle.lifetime = Table[mType].timelife;

	mParticles.push_back(particle);
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commands){

	while (!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
		mParticles.pop_front();

	FOREACH(Particle& particle, mParticles)
		particle.lifetime -= dt;

	mNeedVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	if (mNeedVertexUpdate){
		computeVertex();
		mNeedVertexUpdate = false;
	}

	states.texture = &mTexture;
	target.draw(mVertexArray, states);
}

void ParticleNode::computeVertex() const {
	sf::Vector2f size(mTexture.getSize());
	sf::Vector2f half = size / 2.f;

	mVertexArray.clear();

	FOREACH(const Particle& particle, mParticles){
		sf::Vector2f pos = particle.position;
		sf::Color c = particle.color;

		float ratio = particle.lifetime.asSeconds() / Table[mType].timelife.asSeconds();
		c.a = static_cast<sf::Uint8>(255*std::max(ratio,0.f));

		addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, c);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, c);
		addVertex(pos.x + half.x, pos.y + half.y, size.y, size.y, c);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, c);

	}
}

void ParticleNode::addVertex(float worldX, float worldY, float textCoordX, float textCoordY, sf::Color& color) const {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(textCoordX, textCoordY);
	vertex.color = color;

	mVertexArray.append(vertex);
}

Particle::Type ParticleNode::getParticleType() const{
	return mType;
}

unsigned int ParticleNode::getCategory() const {
	return Category::ParticleSystem;
}
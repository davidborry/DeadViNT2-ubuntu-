#ifndef BLOOMEFFECT_HPP
#define BLOOMEFFECT_HPP

#include "PostEffect.hpp"
#include "../util/ResourceIdentifier.hpp"
#include "../util/ResourceHolder.hpp"
#include <array>

class BloomEffect : public PostEffect{
public:
	BloomEffect();
	virtual void apply(const sf::RenderTexture& intput, sf::RenderTarget& output);

private:
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

private:
	void prepareTexture(sf::Vector2u size);

	void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void blurMultipass(RenderTextureArray& renderTextures);
	void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void downSample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

private:
	ShaderHolder mShaders;

	sf::RenderTexture mBrightnessTexture;
	RenderTextureArray mFirstPassTextures;
	RenderTextureArray mSecondPassTextures;
};

#endif
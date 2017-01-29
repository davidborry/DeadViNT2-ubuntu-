#include "../../headers/graphics/BloomEffect.hpp"

BloomEffect::BloomEffect() :
mShaders(),
mBrightnessTexture(),
mFirstPassTextures(),
mSecondPassTextures()
{
	mShaders.load(Resources::Shaders::AddPass, "Resources/shaders/Fullpass.vert", "Resources/shaders/Add.frag");
	mShaders.load(Resources::Shaders::BrightnessPass, "Resources/shaders/Fullpass.vert", "Resources/shaders/Brightness.frag");
	mShaders.load(Resources::Shaders::DownSamplePass, "Resources/shaders/Fullpass.vert", "Resources/shaders/DownSample.frag");
	mShaders.load(Resources::Shaders::GaussianBlurPass, "Resources/shaders/Fullpass.vert", "Resources/shaders/GuassianBlur.frag");
}

void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output){
	prepareTexture(input.getSize());

	filterBright(input, mBrightnessTexture);

	downSample(mBrightnessTexture, mFirstPassTextures[0]);
	blurMultipass(mFirstPassTextures);

	downSample(mFirstPassTextures[0], mSecondPassTextures[0]);
	blurMultipass(mSecondPassTextures);

	add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
	mFirstPassTextures[1].display();
	add(input, mFirstPassTextures[1], output);
}

void BloomEffect::prepareTexture(sf::Vector2u size){
	if (mBrightnessTexture.getSize() != size){
		mBrightnessTexture.create(size.x, size.y);
		mBrightnessTexture.setSmooth(true);

		mFirstPassTextures[0].create(size.x / 2, size.y / 2);
		mFirstPassTextures[0].setSmooth(true);
		mFirstPassTextures[1].create(size.x / 2, size.y / 2);
		mFirstPassTextures[1].setSmooth(true);

		mSecondPassTextures[0].create(size.x / 4, size.y / 4);
		mSecondPassTextures[0].setSmooth(true);
		mSecondPassTextures[1].create(size.x / 4, size.y / 4);
		mSecondPassTextures[1].setSmooth(true);
		sf::Shader a;
		
	}
}

void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output){
	sf::Shader& brightness = mShaders.get(Resources::Shaders::BrightnessPass);

	brightness.setParameter("source", input.getTexture());
	applyShader(brightness, output);
	output.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures){
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count){
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor){
	sf::Shader& gaussianBlur = mShaders.get(Resources::Shaders::GaussianBlurPass);
	
	gaussianBlur.setParameter("source", input.getTexture());
	gaussianBlur.setParameter("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}

void BloomEffect::downSample(const sf::RenderTexture& input, sf::RenderTexture& output){
	sf::Shader& downSampler = mShaders.get(Resources::Shaders::DownSamplePass);

	downSampler.setParameter("source", input.getTexture());
	downSampler.setParameter("sourceSize", sf::Vector2f(input.getSize()));
	applyShader(downSampler, output);
	output.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output){
	sf::Shader& adder = mShaders.get(Resources::Shaders::AddPass);

	adder.setParameter("source", source.getTexture());
	adder.setParameter("bloom", bloom.getTexture());
	applyShader(adder, output);
}
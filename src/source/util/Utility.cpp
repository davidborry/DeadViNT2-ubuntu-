#include "../../headers/util/Utility.hpp"
#include "../../headers/graphics/Animation.hpp"
#include "SFML/Graphics/Rect.hpp"
#include <random>

namespace{
	std::default_random_engine createRandomEngine(){
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}
void centerOrigin(sf::Sprite& sprite){
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text){
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(Animation& animation){
	sf::FloatRect bounds = animation.getLocalBounds();
	animation.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

float toDegree(float radian)
{
	return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
	return 3.141592653589793238462643383f / 180.f * degree;
}

float length(sf::Vector2f vector){
	return std::sqrt(vector.x*vector.x + vector.y*vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector){
	assert(vector != sf::Vector2f(0.f, 0.f));
	return vector / length(vector);
}

int randomInt(int exclusiveMax){
	std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
	return distr(RandomEngine);
}

sf::FloatRect unionRect(sf::FloatRect rect1, sf::FloatRect rect2){
	float left = std::max(rect1.left, rect2.left);
	float top = std::max(rect1.top, rect2.top);
	float right = std::min(rect1.left + rect1.width, rect2.left + rect2.width);
	float bottom = std::min(rect1.top + rect1.height, rect2.top + rect2.height);

	float width = right - left;
	float height = bottom - top;
	return sf::FloatRect(left, top,width,height);
}

double heuristic(int x1, int y1, int x2, int y2){
	//printf("%d\n", std::abs(x1 - x2) + std::abs(y1 - y2));
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}
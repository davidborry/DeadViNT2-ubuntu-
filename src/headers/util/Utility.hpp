#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <sstream>
#include "SFML/Graphics.hpp"
#include <cassert>

namespace sf{
	class Sprite;
	class Text;
}

class Animation;

struct Point{
	Point(int x,int y);
	int x;
	int y;
};

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template<typename T>
std::string toString(const T& value){
	std::stringstream stream;
	stream << value;
	return stream.str();
}

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

float toDegree(float radian);
float toRadian(float degree);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

int randomInt(int exclusiveMax);

sf::FloatRect unionRect(sf::FloatRect rect1, sf::FloatRect rect2);

double heuristic(int x1, int y1, int x2, int y2);

#endif
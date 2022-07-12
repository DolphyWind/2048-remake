#pragma once
#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.h"
#include <sstream>
#include <iostream>
using namespace std;

typedef unsigned long long int Decimal;

template<typename T>
string toStr(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}

class Unit
{
private:
	int value = 0;
	sf::Color unitColor;
	sf::Color textColor;
	sf::RoundedRectangleShape shape;
	sf::Text text;
	void setColor();
public:
	Unit();
	Unit(sf::Font *font, sf::Vector2f size, Decimal val = 0);
	void setValue(Decimal val);
	void draw(sf::RenderWindow *window);
	void setPosition(sf::Vector2f pos);
};

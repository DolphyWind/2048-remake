#include "units.h"

sf::Color blackTextColor = sf::Color(34, 34, 34);
sf::Color whiteTextColor = sf::Color(249, 246, 242);

sf::Color tile_2 = sf::Color(238, 228, 218);
sf::Color tile_4 = sf::Color(237, 224, 200);
sf::Color tile_8 = sf::Color(242, 177, 121);
sf::Color tile_16 = sf::Color(245, 149, 99);
sf::Color tile_32 = sf::Color(246, 124, 95);
sf::Color tile_64 = sf::Color(246, 94, 59);
sf::Color tile_128 = sf::Color(237, 207, 114);
sf::Color tile_256 = sf::Color(237, 204, 97);
sf::Color tile_512 = sf::Color(237, 200, 80);
sf::Color tile_1024 = sf::Color(237, 197, 63);
sf::Color tile_2048 = sf::Color(237, 194, 46);
sf::Color tile_high = sf::Color(60, 58, 50);
float characterSize = 50;

Unit::Unit() {}

Unit::Unit(sf::Font* font, sf::Vector2f size, Decimal val)
{
	shape = sf::RoundedRectangleShape(size, 5, 90);
	value = val;
	setColor();
	text.setFont(*font);
	text.setCharacterSize(characterSize);
	text.setString(toStr(value));
}

void Unit::setColor()
{
	if (value == 2 || value == 4) textColor = blackTextColor;
	else textColor = whiteTextColor;
	switch (value) {
	case 2:
		unitColor = tile_2;
		break;
	case 4:
		unitColor = tile_4;
		break;
	case 8:
		unitColor = tile_8;
		break;
	case 16:
		unitColor = tile_16;
		break;
	case 32:
		unitColor = tile_32;
		break;
	case 64:
		unitColor = tile_64;
		break;
	case 128:
		unitColor = tile_128;
		break;
	case 256:
		unitColor = tile_256;
		break;
	case 512:
		unitColor = tile_512;
		break;
	case 1024:
		unitColor = tile_1024;
		break;
	case 2048:
		unitColor = tile_2048;
		break;
	default:
		unitColor = tile_high;
		break;
	}

	text.setFillColor(textColor);
	shape.setFillColor(unitColor);
}

void Unit::setValue(Decimal val)
{
	value = val;
	text.setString(toStr(val));
	setColor();
	setPosition(shape.getPosition());
}

void Unit::setPosition(sf::Vector2f pos)
{
	shape.setPosition(pos);
	float div = 30.f / (float)characterSize;
	text.setPosition(pos.x + (shape.getSize().x - text.getLocalBounds().width) / 2 - text.getString().getSize() * 2, pos.y + ((float)text.getLocalBounds().height * div) + 2);
}

void Unit::draw(sf::RenderWindow* window)
{
	if (value != 0)
	{
		window->draw(shape);
		window->draw(text);
	}
}

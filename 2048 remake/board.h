#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include <iostream>
#include <sstream>
#include <array>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;

typedef array<array<int, 4>, 4> Matrix4x4;
typedef array<array<sf::Vector2f, 4>, 4> PosMatrix;

class Board
{
private:
	Matrix4x4 board;
	PosMatrix unitPositions;
	array<array<Unit, 4>, 4> units;
	sf::Vector2f size;
	sf::Font *font;
	Matrix4x4 makeBoard(array<vector<int>, 4> lines, int dir);
public:
	Board(PosMatrix positions, sf::Font &f, sf::Vector2f size);
	void setBoard(Matrix4x4 newBoard);
	Matrix4x4 getBoard();
	void createRandom(int times = 1);
	void printBoardInFile();
	void draw(sf::RenderWindow *window);
	void createUnits();
	Decimal moveToLeft();
	Decimal moveToRight();
	Decimal moveToUp();
	Decimal moveToDown();
	int getEmptyUnitCount();
	bool isGameOver();
	array<vector<int>, 4> getHorizontalLines();
	array<vector<int>, 4> getVerticalLines();
};

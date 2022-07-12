#include "board.h"

Board::Board(PosMatrix positions, sf::Font &f, sf::Vector2f size)
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
			board[y][x] = 0;
	}
	if((rand() % 10) != 9)
		createRandom(1);
	else createRandom(2);
	
	unitPositions = positions;
	this->size = size;
	font = &f;
	
	createUnits();
}

void Board::createUnits()
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			units[y][x] = Unit(font, size, board[y][x]);
			units[y][x].setPosition(unitPositions[y][x]);
		}
	}
}

void Board::createRandom(int times)
{
	if(times == 0) return;
	int randomx = rand() % 4;
	int randomy = rand() % 4;
	if(board[randomy][randomx] != 0) return createRandom(times);
	
	if((rand() % 10) != 9)
		board[randomy][randomx] = 2;
	else board[randomy][randomx] = 4;
	createRandom(times - 1);
}

void Board::printBoardInFile()
{
	ofstream writer("board.txt");
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
			writer << board[y][x] << " ";
		writer << endl;
	}
	writer.close();
}

void Board::draw(sf::RenderWindow *window)
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			units[y][x].draw(window);
		}
	}
}

int Board::getEmptyUnitCount()
{
	int empty = 0;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			if(board[y][x] == 0) empty ++;
		}
	}
	return empty;
}

Matrix4x4 Board::getBoard()
{
	return board;
}

array<vector<int>, 4> Board::getHorizontalLines()
{
	array<vector<int>, 4> lines;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			if(board[y][x] != 0) lines[y].push_back(board[y][x]);
		}
	}
	return lines;
}

array<vector<int>, 4> Board::getVerticalLines()
{
	array<vector<int>, 4> lines;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			if(board[x][y] != 0) lines[y].push_back(board[x][y]);
		}
	}
	return lines;
}

void Board::setBoard(Matrix4x4 newBoard)
{
	board = newBoard;
	createUnits();
}

Matrix4x4 Board::makeBoard(array<vector<int>, 4> lines, int dir)// dir = 0 is left, dir = 1 is right
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < lines[i].size(); j++)
		{
			if(lines[i][j] == 0) lines[i].erase(lines[i].begin() + j);
		}
	}
	vector<int> temp;
	for(int i = 0; i < 4; i++)
	{
		temp.clear();
		int size = lines[i].size();
		if(dir == 0)
		{
			for(int j : lines[i]) temp.push_back(j);
			for(int j = 0; j < 4 - size; j++) temp.push_back(0);
		}
		else if(dir
		 == 1)
		{
			for(int j = 0; j < 4 - size; j++) temp.push_back(0);
			for(int j : lines[i]) temp.push_back(j);
		}
		lines[i] = temp;
	}
	Matrix4x4 result;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			result[y][x] = lines[y][x];
		}
	}
	return result;
}

Decimal Board::moveToLeft()
{
	Matrix4x4 oldBoard = board;
	array<vector<int>, 4> lines = getHorizontalLines();
	int score = 0;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 1; j < lines[i].size(); j++)
		{
			if(lines[i][j - 1] == lines[i][j])
			{
				lines[i][j - 1] *= 2;
				lines[i][j] = 0;
				score += lines[i][j - 1];
			}
		}
	}
	
	Matrix4x4 newBoard = makeBoard(lines, 0);
	if(oldBoard == newBoard) return -1;
	setBoard(newBoard);
	
	if(getEmptyUnitCount() == 0);
	else if(getEmptyUnitCount() == 1) createRandom(1);
	else
	{
		if(rand() % 10 != 9) createRandom(1);
		else createRandom(2);
	}
	createUnits();
	return score;
}

Decimal Board::moveToRight()
{
	Matrix4x4 oldBoard = board;
	array<vector<int>, 4> lines = getHorizontalLines();
	int score = 0;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = lines[i].size() - 2; j >= 0; j--)
		{	
			if(lines[i][j + 1] == lines[i][j])
			{
				lines[i][j + 1] *= 2;
				lines[i][j] = 0;
				score += lines[i][j + 1];
			}
		}
	}
	
	Matrix4x4 newBoard = makeBoard(lines, 1);
	if(oldBoard == newBoard) return -1;
	setBoard(newBoard);
	
	if(getEmptyUnitCount() == 0);
	else if(getEmptyUnitCount() == 1) createRandom(1);
	else
	{
		if(rand() % 10 != 9) createRandom(1);
		else createRandom(2);
	}
	createUnits();
	return score;
}

Decimal Board::moveToUp()
{
	Matrix4x4 oldBoard = board;
	array<vector<int>, 4> lines = getVerticalLines();
	int score = 0;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 1; j < lines[i].size(); j++)
		{
			if(lines[i][j - 1] == lines[i][j])
			{
				lines[i][j - 1] *= 2;
				lines[i][j] = 0;
				score += lines[i][j - 1];
			}
		}
	}
	
	Matrix4x4 newBoard = makeBoard(lines, 0);
	Matrix4x4 temp;
	
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			temp[y][x] = newBoard[x][y];
		}
	}
	newBoard = temp;
	if(oldBoard == newBoard) return -1;
	setBoard(newBoard);
	
	if(getEmptyUnitCount() == 0);
	else if(getEmptyUnitCount() == 1) createRandom(1);
	else
	{
		if(rand() % 10 != 9) createRandom(1);
		else createRandom(2);
	}
	createUnits();
	return score;
}

Decimal Board::moveToDown()
{
	Matrix4x4 oldBoard = board;
	array<vector<int>, 4> lines = getVerticalLines();
	int score = 0;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = lines[i].size() - 2; j >= 0; j--)
		{	
			if(lines[i][j + 1] == lines[i][j])
			{
				lines[i][j + 1] *= 2;
				lines[i][j] = 0;
				score += lines[i][j + 1];
			}
		}
	}
	
	Matrix4x4 newBoard = makeBoard(lines, 1);
	Matrix4x4 temp;
	
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			temp[y][x] = newBoard[x][y];
		}
	}
	newBoard = temp;
	if(oldBoard == newBoard) return -1;
	setBoard(newBoard);
	
	if(getEmptyUnitCount() == 0);
	else if(getEmptyUnitCount() == 1) createRandom(1);
	else
	{
		if(rand() % 10 != 9) createRandom(1);
		else createRandom(2);
	}
	createUnits();
	return score;
}

bool Board::isGameOver()
{
	if(getEmptyUnitCount() > 0) return false;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			if(x - 1 >= 0){
				if(board[y][x] == board[y][x - 1]) return false;
			}
			if(y - 1 >= 0){
				if(board[y][x] == board[y - 1][x]) return false;
			}
			
			if(x + 1 < 4){
				if(board[y][x] == board[y][x + 1]) return false;
			}
			if(y + 1 < 4){
				if(board[y][x] == board[y + 1][x]) return false;
			}	
		}
	}
	return true;
}

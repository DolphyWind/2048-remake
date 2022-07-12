#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>
#include "data.h"
#include "RoundedRectangleShape.h"
#include "colors.h"
#include "variables.h"
#include "board.h"
#include <array>
#include <windows.h>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include "game.h"
using namespace std;

string appdata = getenv("APPDATA");

bool isFolderExists(string folderName)
{
	struct stat info;
	if (stat(folderName.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
		return true;
	else
		return false;
}

struct SaveGameData
{
	Matrix4x4 lastBoard;
	Decimal best;
	Decimal score;
};

void createSaveFolder()
{
	if (!isFolderExists(appdata + "\\2048_data"))
		CreateDirectoryA((appdata + "\\2048_data").c_str(), NULL);
}

string save_file_path = appdata + "\\2048_data\\game.data";

bool readFromFile(SaveGameData& gameData, vector<Matrix4x4>& boards, vector<Decimal>& scores)
{
	ifstream reader(save_file_path, ios::out | ios::binary);
	if (reader.good())
	{
		reader.read((char*)&gameData, sizeof(SaveGameData));
		size_t size;
		reader.read((char*)&size, sizeof(size_t));
		boards = vector<Matrix4x4>(size);
		reader.read((char*)&boards[0], size * sizeof(Matrix4x4));
		reader.read((char*)&size, sizeof(size_t));
		scores = vector<Decimal>(size);
		reader.read((char*)&scores[0], size * sizeof(Decimal));
		reader.close();
		return true;
	}
	return false;
}

void writeToFile(SaveGameData& saveData, vector<Matrix4x4>& boards, vector<Decimal>& scores)
{
	ofstream writer(save_file_path, ios::out | ios::binary);
	writer.write((char*)&saveData, sizeof(SaveGameData));
	size_t size = boards.size();
	writer.write((char*)&size, sizeof(size_t));
	writer.write((char*)&boards[0], boards.size() * sizeof(Matrix4x4));
	size = scores.size();
	writer.write((char*)&size, sizeof(size_t));
	writer.write((char*)&scores[0], scores.size() * sizeof(Decimal));
	writer.close();
}

int main(int argc, char **argv)
{
	auto window_finder = FindWindowA(nullptr, "2048 remake\n");
	if (window_finder != nullptr)
	{
		SetForegroundWindow(window_finder);
		return 0;
	}

	vector<Matrix4x4> boards;
	vector<Decimal> scores;

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "2048 remake\n", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Image window_icon;
	window_icon.loadFromMemory(&icon_data[0], icon_data.size());
	window.setIcon(window_icon.getSize().x, window_icon.getSize().y, window_icon.getPixelsPtr());

	sf::Font font;
	font.loadFromMemory(&font_data[0], font_data.size());

	sf::Texture square_t;
	square_t.loadFromMemory(&square_data[0], square_data.size());
	sf::Sprite square(square_t);

	sf::RoundedRectangleShape header(headerSize, corner_radius, point_count);
	header.setPosition(headerPosition);
	header.setFillColor(headerColor);

	sf::Text headerText("2048", font);
	headerText.setCharacterSize(headerTextCharSize);
	headerText.setPosition((headerPosition.x + (headerSize.x / 2)) - headerText.getGlobalBounds().width / 2, (headerPosition.y + (headerSize.y / 2)) - headerText.getGlobalBounds().height);

	sf::Text debugText("", font);
	debugText.setString(toString(""));
	debugText.setFillColor(sf::Color::Black);

	sf::RoundedRectangleShape score(scoreSize, corner_radius, point_count);
	score.setPosition(scorePosition);
	score.setFillColor(scoreColor);

	sf::Text scoreTopText("Score", font);
	scoreTopText.setCharacterSize(scoreTextCharSize);
	scoreTopText.setPosition(scorePosition.x + (scoreSize.x - scoreTopText.getGlobalBounds().width) / 2, scorePosition.y + scoreTopText.getGlobalBounds().height / 100);
	scoreTopText.setFillColor(textColor);

	sf::Text scoreText("0", font);
	scoreText.setCharacterSize(scoreTextCharSize);
	scoreText.setPosition(scorePosition.x + (scoreSize.x - scoreText.getGlobalBounds().width) / 2, scorePosition.y + 13 * scoreText.getGlobalBounds().height / 7);
	scoreText.setFillColor(textColor);

	sf::RoundedRectangleShape best(bestSize, corner_radius, point_count);
	best.setPosition(bestPosition);
	best.setFillColor(bestColor);

	sf::Text bestTopText("Best", font);
	bestTopText.setCharacterSize(bestTextCharSize);
	bestTopText.setPosition(bestPosition.x + (bestSize.x - bestTopText.getGlobalBounds().width) / 2, bestPosition.y + bestTopText.getGlobalBounds().height / 100);
	bestTopText.setFillColor(textColor);

	sf::RoundedRectangleShape newButton(newButtonSize, corner_radius, point_count);
	newButton.setPosition(newButtonPosition);
	newButton.setFillColor(newButtonColor);

	sf::Text bestText("0", font);
	bestText.setCharacterSize(bestTextCharSize);
	bestText.setPosition(bestPosition.x + (bestSize.x - bestText.getGlobalBounds().width) / 2, bestPosition.y + 13 * bestText.getGlobalBounds().height / 7);
	bestText.setFillColor(textColor);

	sf::Text newButtonText("New", font);
	newButtonText.setCharacterSize(newButtonTextCharSize);
	newButtonText.setPosition(newButtonPosition.x + (newButtonSize.x - newButtonText.getGlobalBounds().width) / 2, newButtonPosition.y + (newButtonSize.y / 2 - newButtonText.getGlobalBounds().height));
	newButtonText.setFillColor(textColor);

	sf::RoundedRectangleShape newButtonMask(newButtonSize, corner_radius, point_count);
	newButtonMask.setPosition(newButtonPosition);
	newButtonMask.setFillColor(maskColor);

	sf::RoundedRectangleShape undoButton(undoButtonSize, corner_radius, point_count);
	undoButton.setPosition(undoButtonPosition);
	undoButton.setFillColor(undoButtonColor);

	sf::Text undoButtonText("Undo", font);
	undoButtonText.setCharacterSize(undoButtonTextCharSize);
	undoButtonText.setPosition(undoButtonPosition.x + (undoButtonSize.x - undoButtonText.getGlobalBounds().width) / 2, undoButtonPosition.y + (undoButtonSize.y / 2 - undoButtonText.getGlobalBounds().height));
	undoButtonText.setFillColor(textColor);

	sf::RoundedRectangleShape undoButtonMask(undoButtonSize, corner_radius, point_count);
	undoButtonMask.setPosition(undoButtonPosition);
	undoButtonMask.setFillColor(maskColor);

	sf::RoundedRectangleShape boardBackground(boardSize, corner_radius, point_count);
	boardBackground.setPosition(boardPosition);
	boardBackground.setFillColor(boardBackgroundColor);

	sf::RoundedRectangleShape gameOverShape(boardSize, corner_radius, point_count);
	gameOverShape.setPosition(boardPosition);
	gameOverShape.setFillColor(noneColor);

	sf::Text aboutText(sf::String(L"Made by Yunus Emre Aydýn."), font);
	aboutText.setCharacterSize(15);
	aboutText.setPosition(headerPosition.x, windowSize.y - aboutText.getGlobalBounds().height * 1.5);
	aboutText.setFillColor(sf::Color::Black);

	array<array<sf::RoundedRectangleShape, 4>, 4> units;
	array<array<sf::Vector2f, 4>, 4> unitPositions;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			units[y][x] = sf::RoundedRectangleShape(unitSize, corner_radius, point_count);
			unitPositions[y][x] = sf::Vector2f(boardPosition.x + (x + 1) * unit_dist + x * unitSize.x, boardPosition.y + (y + 1) * unit_dist + y * unitSize.y);
			units[y][x].setPosition(unitPositions[y][x]);
			units[y][x].setFillColor(unitColor);
		}
	}

	createSaveFolder();

	Decimal scoreVal = 0;
	Decimal bestVal = 0;

	SaveGameData gameData;
	bool dataRead = readFromFile(gameData, boards, scores);

	if (dataRead)
	{
		scoreVal = gameData.score;
		bestVal = gameData.best;
	}

	scoreText.setString(toString(scoreVal));
	bestText.setString(toString(bestVal));

	Board game(unitPositions, font, unitSize);
	if (dataRead && scoreVal > 0) game.setBoard(gameData.lastBoard);

	bool lKeyPress = true;
	bool rKeyPress = true;
	bool uKeyPress = true;
	bool dKeyPress = true;
	bool f2press = true;
	bool lPress = true;

	sf::Vector2i mousePos;
	sf::RectangleShape mousePointer(sf::Vector2f(1, 1));
	mousePointer.setOrigin(0.5, 0.5);

	if (!dataRead || boards.size() <= 1)
	{
		boards.clear();
		scores.clear();
		boards.push_back(game.getBoard());
		scores.push_back(scoreVal);
	}

	bool gameOver = false;
	bool savedToFile = false;

	int alpha = 0;
	int maxAlpha = 200;

	sf::Text gameOverText("Game Over!", font);
	gameOverText.setCharacterSize(45);
	gameOverText.setPosition(boardPosition.x + (boardSize.x - gameOverText.getGlobalBounds().width) / 2, boardPosition.y + boardSize.y / 4 - gameOverText.getGlobalBounds().height * (30.f / (float)gameOverText.getCharacterSize()));
	gameOverText.setFillColor(noneColor);

	HWND hwnd;
	hwnd = window.getSystemHandle();

	while (window.isOpen())
	{
		char title[500];
		GetWindowTextA(hwnd, title, 500);
		if (title != "2048 remake\n") window.setTitle("2048 remake\n");

		mousePos = sf::Mouse::getPosition(window);
		mousePointer.setPosition(sf::Vector2f(mousePos.x, mousePos.y));
		if (!lKeyPress) lKeyPress = !sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		if (!rKeyPress) rKeyPress = !sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		if (!uKeyPress) uKeyPress = !sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		if (!dKeyPress) dKeyPress = !sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

		if (!lPress && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) lPress = true;
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				if (gameOver)
				{
					game = Board(unitPositions, font, unitSize);
					boards.clear();
					scores.clear();
				}
				SaveGameData saveData;
				saveData.score = scoreVal;
				saveData.best = bestVal;
				saveData.lastBoard = game.getBoard();

				writeToFile(saveData, boards, scores);

				window.close();
			}
		}
		scoreText.setPosition(scorePosition.x + (scoreSize.x - scoreText.getGlobalBounds().width) / 2, scorePosition.y + 13 * scoreText.getGlobalBounds().height / 7);
		bestText.setPosition(bestPosition.x + (bestSize.x - bestText.getGlobalBounds().width) / 2, bestPosition.y + 13 * bestText.getGlobalBounds().height / 7);

		if (window.hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
			{
				if (f2press)
				{
					sf::Texture texture;
					texture.create(windowSize.x, windowSize.y);
					texture.update(window);
					sf::Image screenshot = texture.copyToImage();

					time_t now = time(0);
					tm* ltm = localtime(&now);
					int year = 1900 + ltm->tm_year;
					int month = 1 + ltm->tm_mon;
					int day = ltm->tm_mday;
					int hour = ltm->tm_hour;
					int minute = ltm->tm_min;
					int second = ltm->tm_sec;
					string screenshotName = "screenshot " + toString(day) + "-" + toString(month) + "-" + toString(year) + " "
						+ toString(hour) + "." + toString(minute) + "." + toString(second) + ".png";
					screenshot.saveToFile(screenshotName);
					f2press = false;
				}
			}
			else
				f2press = true;
			if (gameOver)
			{
				gameOverShape.setFillColor(sf::Color(0, 0, 0, alpha));
				gameOverText.setFillColor(sf::Color(255, 255, 255, alpha));
				if (alpha < maxAlpha) alpha += 4;
				if (alpha > maxAlpha) alpha = maxAlpha;
			}
			else
			{
				gameOverShape.setFillColor(noneColor);
				gameOverText.setFillColor(noneColor);
				alpha = 0;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (mousePointer.getGlobalBounds().intersects(newButton.getGlobalBounds()) && lPress)
				{
					game = Board(unitPositions, font, unitSize);
					scoreText.setString("0");
					scoreVal = 0;

					scores.clear();
					boards.clear();

					boards.push_back(game.getBoard());
					scores.push_back(scoreVal);
					savedToFile = false;
					gameOver = false;

					lPress = false;
				}
				else if (mousePointer.getGlobalBounds().intersects(undoButton.getGlobalBounds()) && lPress && !gameOver)
				{
					if (boards.size() > 1)
					{
						boards.pop_back();
						scores.pop_back();

						game.setBoard(boards[boards.size() - 1]);
						scoreVal = scores[scores.size() - 1];

						scoreText.setString(toString(scoreVal));
					}

					lPress = false;
				}
			}
			if (mousePointer.getGlobalBounds().intersects(newButton.getGlobalBounds()) && lPress)
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
				newButtonMask.setFillColor(maskColor);
			}
			else newButtonMask.setFillColor(noneColor);

			if (mousePointer.getGlobalBounds().intersects(undoButton.getGlobalBounds()) && lPress)
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
				undoButtonMask.setFillColor(maskColor);
			}
			else undoButtonMask.setFillColor(noneColor);

			bool move = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && lKeyPress && !move && !gameOver)
			{
				Decimal deltaScore = game.moveToLeft();
				if (deltaScore != -1)
				{
					scoreVal += deltaScore;
					scoreText.setString(toString(scoreVal));
					move = true;
				}
				lKeyPress = false;

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && rKeyPress && !move && !gameOver)
			{
				Decimal deltaScore = game.moveToRight();
				if (deltaScore != -1)
				{
					scoreVal += deltaScore;
					scoreText.setString(toString(scoreVal));
					move = true;
				}
				rKeyPress = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && uKeyPress && !move && !gameOver)
			{
				Decimal deltaScore = game.moveToUp();
				if (deltaScore != -1)
				{
					scoreVal += deltaScore;
					scoreText.setString(toString(scoreVal));
					move = true;
				}
				uKeyPress = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dKeyPress && !move && !gameOver)
			{
				Decimal deltaScore = game.moveToDown();
				if (deltaScore != -1)
				{
					scoreVal += deltaScore;
					scoreText.setString(toString(scoreVal));
					move = true;
				}
				dKeyPress = false;
			}
			if (move)
			{
				boards.push_back(game.getBoard());
				scores.push_back(scoreVal);
			}

			if (!gameOver) gameOver = game.isGameOver();
			if (gameOver && !savedToFile)
			{
				if (scoreVal > bestVal) bestVal = scoreVal;
				scoreVal = 0;
				SaveGameData saveData;
				saveData.score = scoreVal;
				saveData.best = bestVal;
				Board tempBoard(unitPositions, font, unitSize);
				saveData.lastBoard = tempBoard.getBoard();
				boards.clear();
				scores.clear();

				writeToFile(saveData, boards, scores);
				bestText.setString(toString(bestVal));
				savedToFile = true;
			}
		}

		window.clear(bgColor);

		window.draw(header);
		window.draw(headerText);

		window.draw(score);
		window.draw(scoreTopText);
		window.draw(scoreText);

		window.draw(best);
		window.draw(bestTopText);
		window.draw(bestText);

		window.draw(newButton);
		window.draw(newButtonText);
		window.draw(newButtonMask);

		window.draw(undoButton);
		window.draw(undoButtonText);
		window.draw(undoButtonMask);

		window.draw(debugText);
		window.draw(boardBackground);
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				window.draw(units[y][x]);
			}
		}
		game.draw(&window);
		window.draw(gameOverShape);
		window.draw(gameOverText);

		window.draw(aboutText);

		window.display();
	}
	return 0;
}

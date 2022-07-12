#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

float horizontal_dist = 35;
float vertical_dist = 20;
float unit_dist = 20;
int point_count = 360 / 4;
int corner_radius = 5;

sf::Vector2f headerSize = sf::Vector2f(150, 150);
sf::Vector2f headerPosition = sf::Vector2f(75, 35);
int headerTextCharSize = floor(headerSize.x * 2 / 7);

sf::Vector2f scoreSize = sf::Vector2f((headerSize.x * 8 / 7), (headerSize.x * 90 / 175));
sf::Vector2f scorePosition = sf::Vector2f(headerPosition.x + headerSize.x + horizontal_dist, headerPosition.y);
int scoreTextCharSize = 28 / 2.22222 * (scoreSize.x / scoreSize.y);

sf::Vector2f bestSize = scoreSize;
sf::Vector2f bestPosition = sf::Vector2f(scorePosition.x + scoreSize.x + horizontal_dist, scorePosition.y);
int bestTextCharSize = 28 / 2.22222 * (bestSize.x / bestSize.y);

sf::Vector2f newButtonSize = sf::Vector2f(scoreSize.x, headerSize.y - scoreSize.y - vertical_dist);
sf::Vector2f newButtonPosition = sf::Vector2f(scorePosition.x, scorePosition.y + scoreSize.y + vertical_dist);
int newButtonTextCharSize = 28 / 3.24324 * (newButtonSize.x / newButtonSize.y);

sf::Vector2f undoButtonSize = newButtonSize;
sf::Vector2f undoButtonPosition = sf::Vector2f(bestPosition.x, newButtonPosition.y);
int undoButtonTextCharSize = 28 / 3.24324 * (undoButtonSize.x / undoButtonSize.y);

sf::Vector2f boardSize = sf::Vector2f(headerSize.x + 2 * horizontal_dist + scoreSize.x + bestSize.x, headerSize.x + 2 * horizontal_dist + scoreSize.x + bestSize.x);
sf::Vector2f boardPosition = sf::Vector2f(headerPosition.x, headerPosition.y + headerSize.y + vertical_dist);

sf::Vector2f unitSize = sf::Vector2f((boardSize.x - 5 * unit_dist) / 4, (boardSize.y - 5 * unit_dist) / 4);

sf::Vector2f windowSize = sf::Vector2f(2 * headerPosition.x + headerSize.x + 2 * horizontal_dist + scoreSize.x + bestSize.x, 2 * headerPosition.y + headerSize.y + vertical_dist + boardSize.y);

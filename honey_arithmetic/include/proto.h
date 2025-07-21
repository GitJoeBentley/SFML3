#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>
#include "HighScores.h"
#include "Sound.h"

float distanceBetweenTwoPositions(sf::Vector2f point1, sf::Vector2f point2);
size_t start(sf::RenderWindow& window, HighScores& highScores, sf::Font& font, Sound& sound);
bool playAgain(sf::RenderWindow& window, sf::Font& font, const std::string& statement, HighScores& highScores, sf::Font& courier);


#endif // PROTO_H_INCLUDED

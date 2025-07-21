#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#include <string>
#include <SFML/Graphics.hpp>

const size_t NumNumbers = 20;
const unsigned NumberFontSize = 48u;
const unsigned CircleRadius = NumberFontSize * 0.75f;

const std::string ResourcePath = "./resources/";
const std::string ArialFont = ResourcePath + "arial.ttf";
const std::string CourierFont = ResourcePath + "CourierNew.ttf";
const std::string HighScoresFile{ResourcePath + "highscores.bin"};

const std::string Pic[4] = {ResourcePath + "honey1.png", ResourcePath + "honey2.png", ResourcePath + "honey3.png", ResourcePath + "honey4.png"};
const sf::Vector2f PicSize = sf::Vector2f(300.f, 400.f);

#endif // CONSTANTS_H_INCLUDED

#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <string>
#include <list>

#include <SFML/Audio.hpp>

#include "HighScores.h"
#include "Invaders.h"
#include "Explosion.h"
#include "Gun.h"
#include "Bullet.h"
#include "Saucer.h"
#include "Bomb.h"
#include "Shield.h"
#include "Sound.h"

enum class Control { Keyboard, Mouse, Joystick};

// Function prototypes
Control start(sf::RenderWindow& window, const HighScores&, Invaders& invaders,
    const sf::Texture& texture, sf::Texture& bombTexture, sf::Texture& gunTexture);
std::string getWelcomeText();
char getKey(const auto* keyPressed);
void startSound(sf::Sound& sound);
void stopSound(sf::Sound& sound);
void displayGameObjects(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, Gun* guns, Invaders& invaders, Explosion& explosion, Bomb* bombPtr, Saucer* saucerPtr, std::list<Bullet*>& bulletsInFlight, sf::Text& gameOverText, Shield* shield);
void pollEvent(sf::RenderWindow& window, const Control& control, Sound& sound, bool& pauseFlag, bool& gameOver, Gun*& guns, std::list<Bullet*>& bulletsInFlight, Score& score);
void manageBullets(std::list<Bullet*>& bulletsInFlight, Shield* shields, Invaders& invaders, Sound& sound, Explosion& explosion,
                   Score& score, Bomb*& bombPtr, bool& gameOver, Saucer*& saucerPtr, sf::RenderWindow& window);
void processGameEnd(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, sf::Font& font,
                    Gun* guns, Invaders& invaders, Shield* shields, const Score& score, HighScores& highScores);

#endif // PROTO_H_INCLUDED

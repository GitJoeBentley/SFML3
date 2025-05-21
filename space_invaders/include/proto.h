#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <string>
#include <SFML/Audio.hpp>

#include "HighScores.h"
#include "Invaders.h"
#include "Explosion.h"
#include "Gun.h"
#include "Bullet.h"
#include "Saucer.h"
#include "Bomb.h"
//#include "Shield.h"
//#include "Sound.h"

enum class Control { Unassigned, Keyboard, Mouse, Joystick};
enum class GameStatus { NotStarted, Paused, Active, GameOver, Quit};

// Forward declarations
class Shield;
class Sound;

// Function prototypes
Control start(sf::RenderWindow& window, const HighScores&, Invaders& invaders,
              const sf::Texture& texture, sf::Texture& bombTexture, sf::Texture& gunTexture);
std::string getWelcomeText();
char getKey(const auto* keyPressed);
void startSound(sf::Sound& sound);
void stopSound(sf::Sound& sound);
void displayGameObjects(sf::RenderWindow& window, sf::RectangleShape& background,
                        sf::Text& text, Gun* guns, Invaders& invaders, Explosion& explosion,
                        Bomb* bombPtr, Saucer* saucerPtr, Bullet*& bullet,
                        sf::Text& gameOverText, Shield* shield);
void pollEvent(sf::RenderWindow& window, const Control& control, Sound& sound,
               GameStatus& status, Gun*& guns, Bullet*& bullet, Score& score);
void manageBullets(Bullet*& bullet, Shield* shields, Invaders& invaders, Sound& sound, Explosion& explosion,
                   Score& score, Bomb*& bombPtr, GameStatus& status, Saucer*& saucerPtr, sf::RenderWindow& window);
void processGameEnd(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, sf::Font& font,
                    Gun* guns, Invaders& invaders, Shield* shields, const Score& score, HighScores& highScores);
bool playAgain(sf::RenderWindow& window, sf::Font& font);


#endif // PROTO_H_INCLUDED

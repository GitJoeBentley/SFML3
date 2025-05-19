#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>

#include "Proto.h"
#include "Constants.h"
#include "Sound.h"

using std::cerr;
using std::cout;
using std::endl;

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(MainWindowWidth, MainWindowHeight)), "Space Invaders", sf::Style::Close);
    window.setTitle(sf::String("Space Invaders"));

    bool pauseFlag = false;
    bool gameOver = false;
    Control control;
    int gunInPlay = 0;
    unsigned loopCount = 0;
    Score score;
    HighScores highScores;

    srand(static_cast<unsigned>(time(0)));

    /////// Background //////
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)));
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(BackgroundFile))
        std::cerr << "Unable to open file " << BackgroundFile << endl;
    background.setTexture(&backgroundTexture);

    if (sf::Joystick::isConnected(0)) cout << "Joystick connected"  << endl;

    // Declare window objects
    Invaders invaders;

    sf::Texture gunTexture(GunImageFile);
    Gun* guns = new Gun[numGuns] { gunTexture, gunTexture, gunTexture };

    sf::Texture explosionTexture(ExplosionImageFile);
    Explosion explosion(explosionTexture);

    std::list<Bullet*> bulletsInFlight;

    Bomb* bombPtr = nullptr;
    sf::Texture bombTexture(BombImageFile);

    sf::Texture saucerTexture(SaucerImageFile);
    Saucer* saucerPtr = nullptr;

    Shield shields[3];

    //////////// Sound effects /////////////
    Sound sound;

    // Declare and load a font
    sf::Font font;
    if (!font.openFromFile(ArielFontFile))
        std::cerr << "Unable to open font file: " << ArielFontFile << endl;

    // Create text
    sf::Text text(font, "", 24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(10,250,50));
    text.setPosition(sf::Vector2f(80.0f,20.0f));

    sf::Text gameOverText(font, "", 32);
    gameOverText.setFillColor(sf::Color(210,250,20));
    gameOverText.setPosition(sf::Vector2f(412.0f,550.0f));

    // Game Speed controls
    float deltaTime = .2f;
    float totalTime = 0.0f;
    sf::Clock clock;

    // Move the second and third gun to spare location
    guns[1].moveToPosition(sf::Vector2f(760.0f+75.0f,30.0f));
    guns[2].moveToPosition(sf::Vector2f(760.0f+150.0f,30.0f));

    control = start(window, highScores, invaders, saucerTexture, bombTexture, gunTexture);
    //cout << "Control is " << (int) control << endl;
    invaders.moveInvadersToStartPosition();

    while (window.isOpen())
    {
        while (gunInPlay < numGuns && !gameOver)
        {
            pollEvent(window, control, sound, pauseFlag, gameOver, guns, bulletsInFlight, score);

            if (!pauseFlag)
            {
                // Drop a bomb at a random interval if no bomb is active
                if (rand() % BombFrequency == 0 && bombPtr == nullptr)
                {
                    sound.start("Bomb");
                    // Every other bomb is a "targeted bomb"
                    int rnd = rand() % 2;
                    if (rnd == 1)
                    {
                        bombPtr = new Bomb(bombTexture);
                        bombPtr -> moveBombToStartPosition(invaders);
                    }
                    else    // drop a targeted bomb
                    {
                        bombPtr = new Bomb(bombTexture, guns[0].getPosition().x);
                        bombPtr->bottomPosition();
                    }
                }
                if (rand()%SaucerFrequency == 0 && saucerPtr == nullptr)
                {
                    saucerPtr = new Saucer(saucerTexture);
                    sound.stop("Invaders");
                    sound.start("Saucer");
                }

                deltaTime = clock.restart().asSeconds();

                totalTime += deltaTime;
                if(totalTime >= SwitchTime)
                {
                    totalTime -= SwitchTime;
                    /// Move the invaders
                    invaders.move();
                    if (invaders.getPositionOfBottomVisibleInvader() >= ShieldYPosition)
                    {
                        gameOver = true;
                    }
                }

                // Move the saucer
                if(saucerPtr)
                {
                    if (!saucerPtr->move())
                    {
                        sound.stop("Saucer");
                        sound.start("Invaders");
                        delete saucerPtr;
                        saucerPtr = nullptr;
                    }
                }

                // Manage the bomb
                if (bombPtr)
                {
                    if (!bombPtr->manage(sound, guns[0], explosion, gunInPlay, gameOver, shields))
                    {
                        delete bombPtr;
                        bombPtr = nullptr;
                    }
                }

                // Manage bullets in flight
                manageBullets(bulletsInFlight, shields, invaders,sound, explosion, score, bombPtr, gameOver, saucerPtr, window);

                // check for explosion
                if (explosion.isExploding())
                {
                    explosion.update();
                    // explosion end
                    if (!explosion.isExploding())
                    {
                        sound.stop("Explosion");
                        sound.start("Invaders");

                        // is it a gun explosion
                        if (fabs(explosion.getPosition().y - guns[0].getPosition().y) < 20.0f)
                        {
                            if (gunInPlay == 2)
                            {
                                gameOver = true;
                                break;
                            }
                            guns[2-gunInPlay].setInvisible();
                            guns[0].setVisible();
                            guns[0].moveToPosition();
                            gunInPlay++;
                        }
                        sf::sleep(sf::Time(sf::seconds(1.0f)));
                    }
                }
            }

            invaders.updateStatus();

            text.setString(sf::String("Score ")+std::to_string(score)+ std::string(66,' ') +"Lives");
            displayWindowObjects(window, background, text, guns, invaders, explosion, bombPtr, saucerPtr, bulletsInFlight, gameOverText, shields);
            loopCount++;
            if (gameOver)
            {
                // stop sounds
                sound.stop("Invaders");
                sound.stop("Saucer");
                sound.stop("Explosion");
                sound.start("Applause");
                gameOverText.setString(sf::String("Game Over"));

                displayWindowObjects(window, background, text, guns, invaders, explosion, bombPtr, saucerPtr, bulletsInFlight, gameOverText, shields);

                //highScores.updateHighScores(Score(name.c_str(),score,time(0)));
                //highScores.WriteHighScoresFile();
                sf::sleep(sf::Time(sf::seconds(5.0f)));   // stall
                window.close();
            }
        }                   // while (gunInPlay < numGuns)
    }                       // while (window.isOpen())
    for (auto i = 0; i < numGuns; i++)
    {
        delete [] guns;
        guns = nullptr;
    }
    return 0;
}

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
using std::string;

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(MainWindowWidth, MainWindowHeight)), "Space Invaders", sf::Style::Close);
    window.setTitle(sf::String("Space Invaders"));

    bool pauseFlag = false;
    bool gameOver = false;
    Control control;
    int gunInPlay = 0;
    unsigned loopCount = 0;
    string statement;
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

    sf::Text gameOverText(font, "", 48);
    gameOverText.setFillColor(sf::Color(0xff0000ff));

    // Game Speed controls
    float deltaTime = .2f;
    float totalTime = 0.0f;
    sf::Clock clock;

    // Move the second and third gun to spare location
    guns[1].moveToPosition(sf::Vector2f(760.0f+75.0f,30.0f));
    guns[2].moveToPosition(sf::Vector2f(760.0f+150.0f,30.0f));

    control = start(window, highScores, invaders, saucerTexture, bombTexture, gunTexture);
    invaders.moveInvadersToStartPosition();

    ///////////////////////////// Game Loop /////////////////////////////////

    while (window.isOpen())
    {
        while (gunInPlay < numGuns && !gameOver)
        {
            pollEvent(window, control, sound, pauseFlag, gameOver, guns, bulletsInFlight, score);

            if (!pauseFlag)
            {
                // Drop a bomb at a random interval if no bomb is active
                int rnd = rand() % 4;

                if (rnd == 3 && rand() % BombFrequency == 0 && bombPtr == nullptr)
                {
                    sound.start("Bomb");
                    // Every other bomb is a "targeted bomb"
                    if (rnd < 2)
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

            }
            // check for explosion
            if (explosion.isExploding())
            {
                explosion.update();
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
                        sf::sleep(sf::Time(sf::seconds(1.0f)));
                    }
                    sf::sleep(sf::Time(sf::seconds(0.2f)));
                }
            }
            invaders.updateStatus();

            text.setString(sf::String("Score ")+std::to_string(score)+ std::string(66,' ') +"Lives");
            displayGameObjects(window, background, text, guns, invaders, explosion, bombPtr, saucerPtr, bulletsInFlight, gameOverText, shields);
            loopCount++;
            if (gameOver)
            {
                // stop sounds
                sound.stop("Invaders");
                sound.stop("Saucer");
                sound.stop("Explosion");
                sound.start("Applause");
                processGameEnd(window, background, text, font, guns, invaders, shields, score, highScores);
                break;
            }
        }                   // while (gunInPlay < numGuns)
        if (gameOver) break;
    }
    for (auto i = 0; i < numGuns; i++)
    {
        delete [] guns;
        guns = nullptr;
    }
    delete guns;
    guns = nullptr;
    if (saucerPtr)
    {
        delete saucerPtr;
        saucerPtr = nullptr;
    }
    if (bombPtr)
    {
        delete bombPtr;
        bombPtr = nullptr;
    }
    window.close();
    return 0;
}

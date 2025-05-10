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
    bool pauseFlag = false;
    bool gameOver = false;
    bool bulletHitsShield = false;
    int invaderHit = 0;
    Score score;
    int gunInPlay = 0;
    unsigned loopCount = 0;
    HighScores highScores;

    srand(static_cast<unsigned>(time(0)));

    /////// Background //////
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)));
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(BackgroundFile))
        std::cerr << "Unable to open file " << BackgroundFile << endl;
    background.setTexture(&backgroundTexture);

    // Declare window objects
    Invaders invaders;

    sf::Texture gunTexture(GunImageFile);
    Gun* guns = new Gun[numGuns] { gunTexture, gunTexture, gunTexture };
    //for (auto i = 0; i < numGuns; i++) guns[i] = new Gun(gunTexture);

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

    std::string name = welcome(window, highScores, invaders, saucerTexture, bombTexture, gunTexture);
    window.setTitle(sf::String(name + "\'s Space Invaders"));
    invaders.moveInvadersToStartPosition();

    sf::Vector2f bombNosePos;
    sf::Vector2f shieldPos;
    sf::Vector2f bulletPos;

    while (window.isOpen())
    {
        while (gunInPlay < numGuns && !gameOver)
        {
            while (const std::optional event = window.pollEvent())
            {
                // Close window: exit
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        sound.stop("Saucer");
                        pauseFlag = !pauseFlag;
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
                        gameOver = true;
                    // Move the gun left or right
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                    {
                        guns[0].move(Gun::Right);
                        break;
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                    {
                        guns[0].move(Gun::Left);
                        break;
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                    {
                        // Limit the number of bullets that can be fired at one time
                        if (bulletsInFlight.size() < 2)
                        {
                            bulletsInFlight.push_back(guns[0].shoot());
                            score += -1;                                   // subtract one point for each bullet
                            sound.start("Bullet");
                        }
                    }
                }
            }
            if (!pauseFlag)
            {
                // Drop a bomb at a random interval if no bomb is active
                if (rand() % BombFrequency == 0 && bombPtr == nullptr)
                {
                    sound.start("Bomb");
                    // Every other bomb is a "targeted bomb"
                    if (rand()%2)
                    {
                        bombPtr = new Bomb(bombTexture);
                        bombPtr -> moveBombToStartPosition(invaders);
                    }
                    else  // drop a targeted bomb
                    {
                        bombPtr = new Bomb(bombTexture, guns[0].getPosition().x);
                        bombPtr->bottomPosition();
                    }
                }
                if (rand()%SaucerFrequency == 0 && saucerPtr == nullptr)    // saucer frequency = 1/8000
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

                // Move the bomb
                if (bombPtr)
                {
                    if (!bombPtr->move())
                    {
                        sound.stop("Bomb");
                        sound.start("Invaders");

                        //startSound(invaderSound);
                        delete bombPtr;
                        bombPtr = nullptr;
                    }
                    else
                    {
                        /// Bomb hits gun?
                        if (guns[0].hitByBomb(*bombPtr))   // gun hit by bomb?
                        {
                            sound.stop("Bomb");
                            sound.start("BombExplosion");

                            delete bombPtr;
                            bombPtr = nullptr;
                            explosion.startExplosion(guns[0].getPosition());
                            if (gunInPlay == 2)
                                gameOver = true;
                        }
                        /// Bomb hits shield
                        else if (bombPtr && bombPtr->bottomPosition() >= 0.9f * MainWindowHeight - ShieldSize.y / 2.0f)
                        {
                            bombNosePos = bombPtr->nosePosition();
                            for (int i = 0; i < 3; ++i)
                            {
                                shieldPos = shields[i].getShield().getPosition();
                                if (shields[i].isAlignedWithBomb(*bombPtr) && bombPtr->bottomPosition() > shields[i].topOfShield() - 10.f)
                                {
                                    sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + (bombNosePos.x-shieldPos.x),22.5f - (shieldPos.y - bombNosePos.y));

                                    if (shields[i].hitByBomb(shieldPositionToExamine))
                                    {
                                        sound.start("Explosion");
                                        // Get rid of the bomb
                                        delete bombPtr;
                                        bombPtr = nullptr;
                                        break;
                                    }
                                    else
                                        break;
                                }
                            }
                        }
                    }
                }
                // move and display the bullets in flight
                for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
                {
                    bulletHitsShield = false;

                    // Bullet moved off the top of the window
                    if (!(*it)->move())
                    {
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);
                        break;
                    }
                    /// Bullet hits shield?
                    for (int i = 0; i < 3; ++i)
                    {
                        if (shields[i].isAlignedWithBullet(**it))
                        {
                            shieldPos = shields[i].getShield().getPosition();
                            bulletPos = (*it)->getPosition();
                            sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + (bulletPos.x-shieldPos.x),22.5f + (bulletPos.y - shieldPos.y));
                            if (shields[i].hitByBullet(shieldPositionToExamine))
                            {
                                // remove bullet
                                delete (*it);
                                *it = nullptr;
                                bulletsInFlight.erase(it);
                                bulletHitsShield = true;
                                break;
                            }
                        }
                    }
                    if (bulletHitsShield)
                        break;

                    /// Bullet hits invader?
                    invaderHit = invaders.invaderHitByBullet(**it);
                    if (invaderHit)
                    {
                        score += invaderHit;
                        sound.start("Explosion");
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);
                        if (invaders.getCountVisible() == 0)
                            gameOver = true;
                        // add a random invader, maybe
                        else if (!gameOver && invaders.getCountVisible() < 10 && rand() % 2  == 0)
                        {
                            invaders.addRandomInvader();
                        }
                        else {};
                        break;
                    }

                    /// Bullet hits bomb?
                    if (bombPtr && bombPtr->hitByBullet(**it))
                    {
                        score += 20;  // 20 points for hitting a bomb
                        //stopSound(bombSound);
                        //startSound(bombExplosionSound);
                        sound.stop("Bomb");
                        sound.start("BombExplosion");
                        explosion.startExplosion(bombPtr->getPosition());

                        // remove bullet
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);

                        // remove the bomb
                        delete bombPtr;
                        bombPtr = nullptr;
                        break;
                    }
                    /// Bullet hits saucer?
                    else if (saucerPtr && saucerPtr->hitByBullet(**it))
                    {
                        score += 10;  // 10 points for hitting a saucer
                        sound.stop("Saucer");
                        sound.start("BombExplosion");
                        explosion.startExplosion(saucerPtr->getPosition());

                        // remove bullet
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);

                        // remove the saucer
                        delete saucerPtr;
                        saucerPtr = nullptr;
                        break;
                    }
                    else
                    {
                        window.draw(**it);
                    }
                }

                // check for explosion
                if (explosion.isExploding())
                {
                    explosion.update();
                    // explosion end
                    if (!explosion.isExploding())
                    {
                        sound.stop("Explosion");
                        sound.start("Invaders");

                        //startSound(invaderSound);

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
                            sf::sleep(sf::Time(sf::seconds(2.0f)));
                        }
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

                highScores.updateHighScores(Score(name.c_str(),score,time(0)));
                highScores.WriteHighScoresFile();
                sf::sleep(sf::Time(sf::seconds(5.0f)));   // stall
                window.close();
            }
        }                          // while (gunInPlay < numGuns)
    }  // while (window.isOpen())
    for (auto i = 0; i < numGuns; i++)
    {
        delete [] guns;
        guns = nullptr;
    }
    return 0;
}


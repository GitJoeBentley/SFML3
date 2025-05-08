#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "Meteor.h"
#include "Star.h"
#include "StationarySprite.h"
#include "CosmicDustParticle.h"

void displayWelcome(sf::RenderWindow& window);

void delay()
{
    for (unsigned long long i = 0; i < 100000; ++i);
}

sf::Vector2f BackgroundSize;
sf::Texture StarTexture;

const std::string ResourcePath = "D:/CB Projects/wandering_star/resources/";

int main()
{
    srand(static_cast<unsigned>(time(0)));
    unsigned long loopCounter = 0;
    const unsigned CosmicDustCount = 100;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024u, 768u)), "Wandering Star", sf::Style::Close | sf::Style::Resize);

    // Background
    sf::RectangleShape background(sf::Vector2f(1024.0f, 768.0f));
    sf::Texture backgroundTexture;
    BackgroundSize = background.getSize();
    if (!backgroundTexture.loadFromFile(ResourcePath + "darksky.png"))
        std::cerr << "Unable to load file: " <<  (ResourcePath + "darksky.png") << std::endl;
    background.setTexture(&backgroundTexture);

    displayWelcome(window);

    // Music
    sf::Music music;
    if (!music.openFromFile(ResourcePath + "testmusic.ogg"))
        std::cerr << "Unable to open " << (ResourcePath + "testmusic.ogg") << std::endl;
    music.setVolume(40);
    music.play();
    sf::Music chirp;
    if (!chirp.openFromFile(ResourcePath + "credulous.ogg"))
        std::cerr << "Unable to open " << (ResourcePath + "credulous.ogg") << std::endl;
    chirp.setVolume(30);
    //chirp.setLoop(true);

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(ResourcePath + "czshhh.wav"))
        std::cerr << "Unable to load " << (ResourcePath + "czshhh.wav") << std::endl;
    sf::Sound sound(buffer);

    ////////////////////////  Sprites  //////////////////////////////

    if (!StarTexture.loadFromFile(ResourcePath + "star.png"))
        std::cerr << "Unable to load star image file" << std::endl;
    Star star;
    Star::Direction starMoveDirection;

    sf::Texture moonTexture;
    if (!moonTexture.loadFromFile(ResourcePath + "moon.png"))
        std::cerr << "Unable to load moon image file" << std::endl;
    StationarySprite moon(moonTexture, 0.8f);

    sf::Texture cometTexture;
    if (!cometTexture.loadFromFile(ResourcePath + "comet.png"))
        std::cerr << "Unable to load comet image file" << std::endl;
    StationarySprite comet(cometTexture, 0.1f);

    CosmicDustParticle cosmicDust[CosmicDustCount];

    Meteor meteor;

    while (window.isOpen())
    {
        delay();
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
                if (keyPressed->scancode == sf::Keyboard::Scancode::M)
                    loopCounter = 95;
                if (keyPressed->scancode == sf::Keyboard::Scancode::C)
                    loopCounter = 999;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                    moon.move(StationarySprite::Right);
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                    moon.move(StationarySprite::Left);
                if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                    comet.move(StationarySprite::Up);
                if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
                    comet.move(StationarySprite::Down);
            }
        }

        ///////////////////  star movement  ////////////////////
        if (loopCounter%10 == 0)
        {
            starMoveDirection = star.move();
            // look for collisions
            if (star.collideWithSprite(moon.getSprite()))
            {
                star.move(star.getOppositeDirection(starMoveDirection));
                std::cout << "Collision with moon" << std::endl;
            }
            if (star.collideWithSprite(comet.getSprite()))
            {
                star.move(star.getOppositeDirection(starMoveDirection));
                std::cout << "Collision with comet" << std::endl;
            }
        }

        // star collisions with cosmic dust particles
        for (auto i = 0u; i < CosmicDustCount; i++)
        {
            if (star.collideWithSprite(cosmicDust[i].getParticle()))
            {
                std::cout << "Cosmic dust particle eaten" << std::endl;
                cosmicDust[i].setAttributes();
                break;
            }
        }

        ///////// Move a random cosmic dust particle  ///////////
        if (loopCounter % 1000 == 0)
        {
            cosmicDust[rand()%CosmicDustCount].setAttributes();
        }

        //////////////  Redraw the window  //////////////////////
        window.clear();
        window.draw(background);
        window.draw(star.getStar());
        window.draw(moon.getSprite());
        window.draw(comet.getSprite());

        //////////////////////// Meteor  ////////////////////////
        if (loopCounter % 5000 == 100)
        {
            sound.play();
            meteor.assignNewPosition();
        }
        if (loopCounter % 5000 > 100 && loopCounter % 5000 < 500)
        {
            meteor.move();
            window.draw(meteor.get_meteor());
        }

        ///////////////////// Cosmic Dust ///////////////////////
        for (auto i=0u; i < CosmicDustCount; i++)
        {
            window.draw(cosmicDust[i].getParticle());
        }

        window.display();

        loopCounter++;
    }
}

void displayWelcome(sf::RenderWindow& window)
{
    std::string textFromFile, buffer;
    std::ifstream fin(ResourcePath + "welcome.txt");
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return;
    }
    while (getline(fin,buffer))
        textFromFile += buffer += '\n';

    fin.close();

    // Text
    sf::Font font;
    if (!font.openFromFile(ResourcePath + "OpenSans-Light.ttf"))
    {
        std::cout << "Can't find font" << std::endl;
    }
    sf::Text text(font,textFromFile.c_str(), 24);
    // set the character size
    // set the color
    text.setFillColor(sf::Color::Red);
    // set the text style
    text.setStyle(sf::Text::Bold);
    text.setPosition(sf::Vector2f(100.0f,50.0f));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                {
                    window.clear();
                    return;
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}


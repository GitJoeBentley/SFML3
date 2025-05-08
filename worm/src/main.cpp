// Hungry Worm by Joe Bentley 12/18, updated 12/19

#define _CRT_SECURE_NO_WARNINGS              // for MSVS 2017

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

#include "Worm.h"
#include "Item.h"
#include "HighScores.h"

std::string ResourcePath = "resources/";
std::string welcome(sf::RenderWindow& window, const HighScores&);
char getKey(const auto*);
std::string getWelcomeText(const HighScores& highScores);
sf::Vector2f BackgroundSize;
sf::String output(int applesEaten, int wallCollisions, int rockCollisions, int score);

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    unsigned long loopCounter = 0;
    const unsigned RockCount = 20;
    bool pauseFlag = false;
    int applesEaten = 0, wallCollisions = 0, rockCollisions = 0, score = 0;
    bool gameOverFlag = false;
    HighScores highScores;
    unsigned frameRate = 8;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024u, 700u)), "", sf::Style::Close);

    // Background
    sf::RectangleShape background(sf::Vector2f(1024.0f, 650.0f));
    sf::Texture backgroundTexture;
    BackgroundSize = background.getSize();
    if (!backgroundTexture.loadFromFile(ResourcePath + "grass.png"))
        cerr << "Unable to open file " << (ResourcePath + "grass.png") << endl;
    background.setTexture(&backgroundTexture);

    std::string name = welcome(window, highScores);
    window.setTitle(sf::String(name + "\'s Hungry Worm"));
    window.setFramerateLimit(frameRate);

    // Declare and load a font
    sf::Font font;
    if (!font.openFromFile(ResourcePath + "arial.ttf"))
        cerr << "Unable to open file " << (ResourcePath + "arial.ttf") << endl;
    // Create a text
    sf::Text text(font,output(applesEaten, wallCollisions, rockCollisions, score), 30);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(10,20,250));
    text.setPosition(sf::Vector2f(10.0f,660.0f));

    sf::Text gameoverText(font, "Game Over", 72);
    gameoverText.setStyle(sf::Text::Bold);
    gameoverText.setFillColor(sf::Color(220,10,40));
    gameoverText.setPosition(sf::Vector2f(BackgroundSize.y/2.0f,BackgroundSize.y/2.0f));

    ////////////////////////  Sprites  //////////////////////////////
    sf::Texture headTexture;
    if (!headTexture.loadFromFile(ResourcePath + "wormhead.png"))
        cerr << "Unable to open file: " << (ResourcePath + "wormhead.png") << endl;
    sf::Texture segmentTexture;
    if (!segmentTexture.loadFromFile(ResourcePath + "wormsegment.png"))
        cerr << "Unable to open file " << (ResourcePath + "wormsegment.png") << endl;

    Worm worm(headTexture, segmentTexture);

    std::vector<Item*> items;

    // Textures
    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile(ResourcePath + "apple.png"))
        cerr << "Unable to open file " << (ResourcePath + "apple.png") << endl;

    sf::Texture rockTexture;
    if (!rockTexture.loadFromFile(ResourcePath + "rock.png"))
        cerr << "Unable to open file " << (ResourcePath + "rock.png") << endl;
    // create apple
    items.push_back(new Item(appleTexture, items));

    // create rocks
    for (unsigned i = 0; i < RockCount; ++i)
        items.push_back(new Item(rockTexture,items, 1.0f/(rand()%4+1)));

    // Sound effects
    sf::SoundBuffer applebiteBuffer;
    if (!applebiteBuffer.loadFromFile(ResourcePath + "apple_bite.wav"))
        cerr << "Unable to open file: " << (ResourcePath + "apple_bite.wav") << endl;
    sf::Sound appleBiteSound(applebiteBuffer);

    sf::SoundBuffer hitthewallBuffer;
    if (!hitthewallBuffer.loadFromFile(ResourcePath + "homer_doh.wav"))
        cerr << "Unable to open file: " << (ResourcePath + "homer_doh.wav") << endl;
    sf::Sound hitthewallSound(hitthewallBuffer);

    sf::SoundBuffer ouchBuffer;
    if (!ouchBuffer.loadFromFile(ResourcePath + "ouch.wav"))
        cerr << "Unable to open file: " << (ResourcePath + "ouch.wav") << endl;
    sf::Sound ouchSound(ouchBuffer);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                case sf::Keyboard::Scancode::Q:
                    gameOverFlag = true;
                    break;
                case sf::Keyboard::Scancode::Escape:
                    pauseFlag = !pauseFlag;
                    break;
                case sf::Keyboard::Scancode::Left:
                    worm.setDirection(Worm::Left);
                    break;
                case sf::Keyboard::Scancode::Right:
                    worm.setDirection(Worm::Right);
                    break;
                case sf::Keyboard::Scancode::Up:
                    worm.setDirection(Worm::Up);
                    break;
                case sf::Keyboard::Scancode::Down:
                    worm.setDirection(Worm::Down);
                default:
                    ;
                }
            }
        }

        if (!pauseFlag)
        {
            worm.move();

            if (worm.hitTheWall())
            {
                hitthewallSound.play();
                wallCollisions++;
                worm.moveOppositeDirection();
            }
            for (size_t i = 0; i < items.size(); ++i)
            {
                if (worm.collideWith(items[i]->getSprite()))
                {
                    if (i == 0) // collided with apple
                    {
                        appleBiteSound.play();
                        applesEaten++;
                        items[0]->move();
                        items.push_back(new Item(rockTexture, items, 1.0f/(rand()%4+1))); // create a new rock
                        frameRate++;
                        window.setFramerateLimit(frameRate);
                        //delayTime = static_cast<unsigned long>(.9*delayTime);  // speed up the game
                        worm.addSegment();
                        break;
                    }
                    else        // collided with rock
                    {
                        ouchSound.play();
                        rockCollisions++;
                        worm.moveOppositeDirection();
                        break;
                    }
                }
            }
            score = 10*applesEaten - wallCollisions - rockCollisions;
            text.setString(output(applesEaten, wallCollisions, rockCollisions, score));

        }
        //////////////  Redraw the window  //////////////////////
        window.clear();
        window.draw(background);
        window.draw(text);
        //  window.draw(apple.getSprite());
        for (auto it = items.begin(); it != items.end(); ++it)
            window.draw((*it)->getSprite());
        worm.draw(window);
        if (gameOverFlag || wallCollisions + rockCollisions == 10)  // Game over
        {
            gameOverFlag = true;
            window.draw(gameoverText);
            highScores.updateHighScores(Score(name.c_str(),applesEaten,score,time(0)));
            highScores.WriteHighScoresFile();
        }
        window.display();
        if (gameOverFlag)
        {
            sf::sleep(sf::Time(sf::seconds(5.0f)));
            window.close();
        }
        loopCounter++;
        //   std::cout << loopCounter << std::endl;
    }
}

sf::String output(int applesEaten, int wallCollisions, int rockCollisions, int score)
{
    std::ostringstream sout;
    sout << "Apples Eaten " << applesEaten << "    Wall Collisions " << wallCollisions
         << "    Rock Collisions " << rockCollisions << "    Score " << score;
    return sout.str();
}

std::string welcome(sf::RenderWindow& window, const HighScores& highScores)
{
    std::string text, buffer,name;

    // Text
    sf::Font font;
    if (!font.openFromFile(ResourcePath + "CourierNew.ttf"))
    {
        std::cout << "Can't find font" << std::endl;
    }
    text = getWelcomeText(highScores);
    sf::Text contents(font, text, 20);
    contents.setFillColor(sf::Color::Cyan);
    contents.setPosition(sf::Vector2f(32.0f,32.0f));

    char input = ' ';
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                input = getKey(keyPressed);
                if ((input >= 'a' && input <= 'z')||(input >= 'A' && input <= 'Z')||input == ' ')
                {
                    name += input;
                }
                if (input == '\b') // backspace
                {
                    name = "";
                    text = getWelcomeText(highScores);
                }
            }
        }
        window.clear();
        contents.setString((text+name+'_').c_str());
        window.draw(contents);
        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = toupper(name[0]);

        if (input == '\n')
            return name;
    }
    return "";
}

// Determine what key is pressed
char getKey(const auto* keyPressed)
{
    if (keyPressed->scancode == sf::Keyboard::Scancode::A)
        return  'a';
    if (keyPressed->scancode == sf::Keyboard::Scancode::B)
        return  'b';
    if (keyPressed->scancode == sf::Keyboard::Scancode::C)
        return  'c';
    if (keyPressed->scancode == sf::Keyboard::Scancode::D)
        return  'd';
    if (keyPressed->scancode == sf::Keyboard::Scancode::E)
        return  'e';
    if (keyPressed->scancode == sf::Keyboard::Scancode::F)
        return  'f';
    if (keyPressed->scancode == sf::Keyboard::Scancode::G)
        return  'g';
    if (keyPressed->scancode == sf::Keyboard::Scancode::H)
        return  'h';
    if (keyPressed->scancode == sf::Keyboard::Scancode::I)
        return  'i';
    if (keyPressed->scancode == sf::Keyboard::Scancode::J)
        return  'j';
    if (keyPressed->scancode == sf::Keyboard::Scancode::K)
        return  'k';
    if (keyPressed->scancode == sf::Keyboard::Scancode::L)
        return  'l';
    if (keyPressed->scancode == sf::Keyboard::Scancode::M)
        return  'm';
    if (keyPressed->scancode == sf::Keyboard::Scancode::N)
        return  'n';
    if (keyPressed->scancode == sf::Keyboard::Scancode::O)
        return  'o';
    if (keyPressed->scancode == sf::Keyboard::Scancode::P)
        return  'p';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
        return  'q';
    if (keyPressed->scancode == sf::Keyboard::Scancode::R)
        return  'r';
    if (keyPressed->scancode == sf::Keyboard::Scancode::S)
        return  's';
    if (keyPressed->scancode == sf::Keyboard::Scancode::T)
        return  't';
    if (keyPressed->scancode == sf::Keyboard::Scancode::U)
        return  'u';
    if (keyPressed->scancode == sf::Keyboard::Scancode::V)
        return  'v';
    if (keyPressed->scancode == sf::Keyboard::Scancode::W)
        return  'w';
    if (keyPressed->scancode == sf::Keyboard::Scancode::X)
        return  'x';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Y)
        return  'y';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Z)
        return  'z';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        return  ' ';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        return  '\n';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
        return  '\b';
    return ' ';
}

std::string getWelcomeText(const HighScores& highScores)
{
    std::ifstream fin(ResourcePath + "welcome.txt");
    std::string text, buffer;
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return "";
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }
    std::ostringstream sout;
    sout << highScores << std::endl;
    text += sout.str();

    text += "Please type your name and press Enter ===> ";

    fin.close();
    return text;
}

#include <iostream>
#include <fstream>
#include <sstream>

#include "Proto.h"
#include "Constants.h"
#include "Gun.h"
#include "Saucer.h"

using std::endl;
using std::cerr;


std::string welcome(sf::RenderWindow& window, const HighScores& highScores, Invaders& invaders,
    const sf::Texture& saucerTexture, sf::Texture& bombTexture, sf::Texture& gunTexture)
{
    std::string text, buffer,name;

       // Music
    sf::Music music;
    if (!music.openFromFile(OpeningMusicFile))
        cerr << "Unable to open " << OpeningMusicFile << endl;
    music.setVolume(40);
    music.play();

    // Text
    sf::Font font;
    if (!font.openFromFile(CourierNewFontFile))
    {
        std::cout << "Can't find font" << std::endl;
    }
    text = getWelcomeText();
    sf::Text instructions(font, text, 20);
    instructions.setFillColor(sf::Color::Cyan);
    instructions.setPosition(sf::Vector2f(90.0f,20.0f));

    // Create High Scores text
    sf::Text highScoresText(font, "", 20);
    highScoresText.setFillColor(sf::Color::Green);
    highScoresText.setPosition(sf::Vector2f(480.0f,210.0f));

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());

    char input = ' ';

    // Create and position a gun
    Gun gun(gunTexture);
    gun.moveToPosition(sf::Vector2f(56.0f,264.0f));

    // Position 5 invaders
    for (auto i = 0; i < 5; i++)
    {
        invaders.getInvader(i,0).setPosition(sf::Vector2f(55.0f,313.0f + i * 45.0f));
    }

    // create and position a saucer
    Saucer* saucerPtr = new Saucer(saucerTexture);
    saucerPtr->setPosition(sf::Vector2f(50.0f,544.0f));

    // create and position 3 bombs
    //Bomb bomb[3] = {{bombTexture,0}, {bombTexture,1},{bombTexture,2}};
    Bomb* bomb[3];

    for (auto i = 0; i < 3; i++)
    {
        bomb[i] = new Bomb(bombTexture, i);
        bomb[i]->setPosition(sf::Vector2f(40.0f+ i*16.0f, 610.0f));
    }

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
                    text = getWelcomeText();
                }
            }
        }

        window.clear();
        instructions.setString((text+name+'_').c_str());
        window.draw(instructions);
        window.draw(highScoresText);

        window.draw(gun.getGun());

        for (auto i = 0; i < 5; i++)
        {
            window.draw(invaders.getInvader(i,0));
        }
        window.draw(*saucerPtr);
        for (auto i = 0; i < 3; i++)
        {
            window.draw(*(bomb[i]));
        }

        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = toupper(name[0]);

        if (input == '\n')
        {
            delete saucerPtr;
            window.clear();
            music.stop();
            return name;
        }
    }
    music.stop();
    delete saucerPtr;
    for (int i = 0; i < 3; i++) delete bomb[i];
    return "";
}

std::string getWelcomeText()
{
    std::ifstream fin(WelcomeFile);
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

    text += "\n          Please type your name and press Enter ===> ";

    fin.close();
    return text;
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

void startSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::Sound::Status::Playing)
        sound.play();
}

void stopSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::Sound::Status::Stopped)
        sound.stop();
}

void displayWindowObjects(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, Gun** guns,
                          Invaders& invaders, Explosion& explosion, Bomb* bombPtr, Saucer* saucerPtr,
                          std::list<Bullet*>& bulletsInFlight, sf::Text& gameOverText, Shield* shields)
{
    window.clear();
    window.draw(background);
    window.draw(text);
    invaders.draw(window);
    if (guns[0]->isVisible())
        window.draw(guns[0]->getGun());
    if (guns[1]->isVisible())
        window.draw(guns[1]->getGun());
    if (guns[2]->isVisible())
        window.draw(guns[2]->getGun());
    if (explosion.isExploding())
        window.draw(explosion.getExplosion());
    if (bombPtr)
        window.draw(*bombPtr);
    if (saucerPtr)
        window.draw(*saucerPtr);
    // draw bullets
    for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
        window.draw(**it);
    window.draw(gameOverText);
    window.draw(shields[0].getShield());
    window.draw(shields[1].getShield());
    window.draw(shields[2].getShield());
    window.display();
}

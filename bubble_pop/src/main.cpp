#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>
using namespace std;

#include "Constants.h"
#include "Bubble.h"
#include "HighScores.h"

sf::Vector2f randomWindowPosition(const Bubble& BigBubble);
sf::Vector2f randomDirection();
float randomSize();
float randomSpeed();
std::string welcome(sf::RenderWindow& window, const HighScores& highScores);
std::string getWelcomeText();
char getKey(const auto* keyPressed);

int main()
{
    srand(time(nullptr));
    bool pauseFlag = false;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024u,720u)), "Bubble Pop", sf::Style::Close);
    window.setFramerateLimit(60);
    unsigned numberOfBubbles = 0;
    HighScores highScores;
    // bool joystickConnected = sf::Joystick::isConnected(0);
    //sf::Vector2f joystick = sf::Vector2f(0.f,0.f);

    // create bottom game border
    sf::RectangleShape bottom(sf::Vector2f(MainWindowSize.x,10.0f));
    bottom.setPosition(sf::Vector2f(0.0f,MainWindowSize.y));
    bottom.setFillColor(sf::Color::Magenta);

    // Create sprites
    sf::Vector2f BigBubblePosition = {MainWindowSize.x/2.0f,MainWindowSize.y/2.0f};
    sf::Texture bubbleTexture;
    if (!bubbleTexture.loadFromFile(BubbleFile))
        cerr << "Unable to open file " << BubbleFile << endl;
    Bubble BigBubble(bubbleTexture,BigBubblePosition, randomDirection(), 1.0f, 1.0f);

    std::list<Bubble*> bubbles;

    //////////// Sound effects /////////////
    sf::SoundBuffer bubbleBuffer;
    if (!bubbleBuffer.loadFromFile(BubbleSoundFile))
        cerr << "Unable to open file " << BubbleSoundFile << endl;
    sf::Sound bubbleSound(bubbleBuffer);

    sf::SoundBuffer bigBubbleBuffer(BigBubbleSoundFile);
    sf::Sound bigBubbleSound(bigBubbleBuffer);
    bigBubbleSound.setVolume(100.0f);

    sf::SoundBuffer endOfGameBuffer;
    if (!endOfGameBuffer.loadFromFile(EndOfGameSoundFile))
        cerr << "Unable to open file " << EndOfGameSoundFile << endl;
    sf::Sound endOfGameSound(endOfGameBuffer);
    bigBubbleSound.setVolume(30.0f);
   // bigBubbleSound.setLoop(true);


    // Music
    sf::Music music;
    if (!music.openFromFile(BubbleSong))
        cerr << "Unable to open file " << BubbleSong << endl;
    music.setVolume(40);
    music.play();

    // Fonts
    sf::Font font;
    if (!font.openFromFile(ArialFont))
        cerr << "Unable to open font file: " << ArialFont << endl;
    // Texts
    sf::Text message(font, "Number of Bubbles " + std::to_string(numberOfBubbles), 24);
    message.setFillColor(sf::Color::Green);
    message.setPosition(sf::Vector2f(400.0f,660.0f));
    auto loopCounter = 0u;
    std::string name = welcome(window, highScores);
    sf::Clock clock;

    ///////  Game Loop ///////
    while (window.isOpen())
    {
        // Compute frame rate
        // float currentTime = clock.restart().asSeconds();
        // float fps = 1.0f / currentTime;

        // std::cout << "fps: " << fps << std::endl;

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
                    window.close();
                    break;
                case sf::Keyboard::Scancode::Escape:
                    pauseFlag = !pauseFlag;
                    break;
                case sf::Keyboard::Scancode::Left:
                    BigBubble.setDirection(sf::Vector2f(-1.0f,0.0f));
                    break;
                case sf::Keyboard::Scancode::Right:
                    BigBubble.setDirection(sf::Vector2f(1.0f,0.0f));
                    break;
                case sf::Keyboard::Scancode::Up:
                    BigBubble.setDirection(sf::Vector2f(0.0f,-1.0f));
                    break;
                case sf::Keyboard::Scancode::Down:
                    BigBubble.setDirection(sf::Vector2f(0.0f,1.0f));
                default:
                    ;
                }
            }
        }
         //   if (sf::Joystick::isButtonPressed(0,0))
           //     BigBubble.setSpeed(BigBubble.getSpeed()*1.2);  // 'A' button pressed (increase speed)
           // if (sf::Joystick::isButtonPressed(0,1))
             //   BigBubble.setSpeed(BigBubble.getSpeed()*0.8);  // 'B' button pressed (decrease speed)


        //joystick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
        //if (fabs(joystick.x) > 15.f || fabs(joystick.y) > 15.f)
       // {
         //   BigBubble.setDirection(sf::Vector2f(joystick.x/100.0f,joystick.y/100.0f));
            //std::cout << joystick.x << ' ' << joystick.y << std::endl;
        //}

        loopCounter++;
        if (!(loopCounter%25))
        {
            //std::cout << "*** Executing loop #" << loopCounter << std::endl;
            bubbles.push_back(new Bubble(bubbleTexture,randomWindowPosition(BigBubble),randomDirection(),randomSize(), randomSpeed()));
            numberOfBubbles++;
        }

        if (!pauseFlag)
        {
            BigBubble.move();
            if (BigBubble.collide(bubbles))
            {
                bigBubbleSound.play();
                sf::sleep(sf::Time(sf::seconds(1.0f)));
                bigBubbleSound.stop();
                window.close();
            }
            // move random bubbles
            for (auto each = bubbles.begin(); each != bubbles.end(); ++each)
            {
                if ((*each)->drift(BigBubble))
                {
                    bubbleSound.setVolume(100.0f * (*each)->getSize());
                    if (bubbleSound.getStatus() != sf::SoundSource::Status::Playing)
                        bubbleSound.play();
                    bubbles.erase(each);
                    numberOfBubbles--;
                    break;
                }
            }
        }
        window.clear();
        window.draw(bottom);
        for (auto each = bubbles.begin(); each != bubbles.end(); ++each)
            window.draw(**each);
        window.draw(BigBubble);
        message.setString("Number of Bubbles " + std::to_string(numberOfBubbles));
        window.draw(message);
        window.display();
    }
    endOfGameSound.play();
    sf::sleep(sf::Time(sf::seconds(3.0f)));   // stall

    highScores.updateHighScores(Score(name.c_str(),numberOfBubbles,time(0)));
    highScores.WriteHighScoresFile();

}

sf::Vector2f randomWindowPosition(const Bubble& BigBubble)
{
    float distanceToTheBubble = 0.0f;
    float deltaX;
    float deltaY;
    sf::Vector2f temp;
    do
    {
        temp.x = rand()% MainWindowSize.x *0.9f;
        temp.y = rand()% MainWindowSize.y *0.9f;
        deltaX = temp.x - BigBubble.getPosition().x;
        deltaY = temp.y - BigBubble.getPosition().y;
        distanceToTheBubble = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }
    while (distanceToTheBubble < BigBubble.width()*BigBubble.getSize());
    return temp;
}


sf::Vector2f randomDirection()
{
    sf::Vector2f temp;
    temp.x = (rand() % 21 -10) / 25.0f;
    temp.y = (rand() % 22 -10) / 25.0f;
    return temp;
}

float randomSpeed()
{
    return 1.0f / (rand() % 19 + 2);
}

float randomSize()
{
    return 1.0f / (rand() % 10 + 2);

}

std::string welcome(sf::RenderWindow& window, const HighScores& highScores)
{
    std::string text, buffer,name;

    // Text
    sf::Font font;
    if (!font.openFromFile(CourierFont))
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
    highScoresText.setPosition(sf::Vector2f(360.0f,300.0f));

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());

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
                    text = getWelcomeText();
                }
            }
        }

        window.clear();
        instructions.setString((text+name+'_').c_str());
        window.draw(instructions);
        window.draw(highScoresText);

        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = toupper(name[0]);

        if (input == '\n')
        {
            window.clear();
            return name;
        }
    }
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
